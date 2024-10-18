#include "mapgeneratorsystem.hpp"

namespace DofM
{
MapGeneratorSystem::MapGeneratorSystem() {}

void MapGeneratorSystem::GenerateRoom(Location::SLocation start, Location::SLocation stop, std::vector<MapObject::SMapObject> &roomvector)
{
    auto minx = start->X;
    auto miny = start->Y;
    auto maxx = stop->X;
    auto maxy = stop->Y;

    for(int ly = miny; ly <= maxy; ly++)
    {
        for(int lx = minx; lx <= maxx; lx++)
        {
            auto t1 = std::make_shared<MapObject>();
            t1->Point = std::make_shared<Location>(lx, ly, 0);
            t1->Tile.TileType = Tile::TileTypes::STONE_INCAVE_GROUND;
            this->NonDuplicateVectorPush(t1, roomvector);
        }
    }
}

bool MapGeneratorSystem::NonDuplicateVectorPush(MapObject::SMapObject mo, std::vector<MapObject::SMapObject> &roomvector)
{
    for(auto &n:roomvector)
    {
        if (mo->Point->IsSame(n->Point))
        {
            fmt::print("Duplicate found {} (in list: {})\n", n->Point->ToString(), mo->Point->ToString());
            //mo->MapSymbol = "-";
            //roomvector.push_back(mo);
            return false;
        }
    }
    roomvector.push_back(mo);
    return true;
}

std::vector<MapObject::SMapObject> MapGeneratorSystem::GenerateMap(std::vector<Location::SLocation> exits)
{
    int maxx = 8, maxy = 8;


    std::vector<MapObject::SMapObject> rval;
    std::vector<Location::SLocation> starttiles;


    for(auto &n : exits)
    {
        if (maxx < n->X)
        {
            maxx = n->X;
        }
        if (maxy < n->Y)
        {
            maxy = n->Y;
        }
    }

    int connecttilex = this->ToolsObject.RndRange(maxx/4, maxx/4*3);
    int connecttiley = this->ToolsObject.RndRange(maxy/4, maxy/4*3);

    auto tilenorth = std::make_shared<MapObject>();
    tilenorth->Point = std::make_shared<Location>(connecttilex,connecttiley,00);
    tilenorth->Tile.TileType = Tile::TileTypes::STONE_INCAVE_GROUND;
    tilenorth->MapSymbol = "T";

    //this->NonDuplicateVectorPush(t1, rval);
    for(auto &n : exits)
    {
        auto t1 = std::make_shared<MapObject>();
        t1->Point = std::make_shared<Location>(n->X,n->Y,0);
        t1->Tile.TileType = Tile::TileTypes::DOOR;
        t1->MapSymbol = "¤";
        this->NonDuplicateVectorPush(t1, rval);
        starttiles.push_back(t1->Point);
        starttiles.push_back(tilenorth->Point);
    }

    this->NonDuplicateVectorPush(tilenorth, rval);
    // Determine pair-number for additional tiles to generate as spawn points for hallway generator
    int tiles = this->ToolsObject.RndRange(0, 0);
    if (( (tiles+starttiles.size()) % 2) != 0)
        tiles++;


    for (int x  = 0; x < tiles; x++)
    {
        int tilesx = this->ToolsObject.RndRange(1, maxx);
        int tilesy = this->ToolsObject.RndRange(1, maxy);
        auto t1 = std::make_shared<MapObject>();
        t1->Point = std::make_shared<Location>(tilesx, tilesy, 0);
        t1->Tile.TileType = Tile::TileTypes::WALL;
        this->NonDuplicateVectorPush(t1, rval);
        starttiles.push_back(t1->Point);
    }



    std::vector<Location::SLocation>::iterator it;


    /*
    // Deleting a element using iterators
    for (i = v.begin(); i != v.end(); ++i) {
        if (i == v.begin() + 1) {
            i = v.erase(i);
            // i now points to the element after the
            // deleted element
        }
    }
    */

    // determine next move, determine cost of next move vs other cost
    // determine if we need to move positive or negative.
    enum ops
    {
        INC,
        DEC
    } xop = ops::INC, yop = ops::INC;

    auto ops_tostring = [](ops op)
    {
        switch(op)
        {
        case ops::INC:
            return std::string("INC");
        case ops::DEC:
            return std::string("DEC");
        }
    };

    for (it = starttiles.begin(); it != starttiles.end(); )
    {
        bool foundwall = false;
        auto firstloc = (*it);
        unsigned int currentx = (*it)->X;
        unsigned int currenty = (*it)->Y;
        it++;
        auto secondloc = (*it);
        unsigned int stopx = (*it)->X;
        unsigned int stopy = (*it)->Y;
        it++;

        fmt::print("Next Tile bundle: {} -> {}\n", firstloc->ToString(), secondloc->ToString());
        xop = ops::INC, yop = ops::INC;
        if (currentx > stopx)
        {
            xop = ops::DEC;
        }
        if (currenty > stopy)
        {
            yop = ops::DEC;
        }
        int retrycount = 0;
        while(!foundwall)
        {
            auto nextloc = std::make_shared<Location>(currentx, currenty, 0);
            if (nextloc->IsSame(secondloc))
            {

                bool enableroomgenerator = false;
                if (this->ToolsObject.RndRange(0,24) > 12 && enableroomgenerator)
                {
                    // Generate room - but clamp the locations to within max/min of the map that is defined by the exits.
                    // or 8 x 8 if lack of exits.
                    int roomendx = this->ToolsObject.RndRange(2, 6);
                    if (nextloc->X + roomendx > maxx)
                        roomendx = roomendx - ((nextloc->X + roomendx) - maxx);
                    int roomendy = this->ToolsObject.RndRange(2, 6);
                    if (nextloc->Y + roomendy > maxy)
                        roomendy = roomendy - ((nextloc->Y + roomendy) - maxy);

                    auto roomendloc = nextloc->OffsetLocation(roomendx, roomendy, 0);
                    fmt::print("Generate room on {} x {} -> {} x {}\n", nextloc->X, nextloc->Y, roomendloc->X, roomendloc->Y);
                    this->GenerateRoom(nextloc, roomendloc, rval);
                }

                foundwall = true; // could be a break, we will go back to outer loop
                continue;
            }
            else
            {

                auto t1 = std::make_shared<MapObject>();
                t1->Point = std::make_shared<Location>(currentx, currenty, 0);
                t1->Tile.TileType = Tile::TileTypes::STONE_INCAVE_GROUND;
                fmt::print("Adding: {} -> {}\n", t1->Point->ToString(), secondloc->ToString());
                this->NonDuplicateVectorPush(t1, rval);


                bool xatend = currentx == stopx;
                bool yatend = currenty == stopy;

                if (xatend && yatend)
                {
                    foundwall = true;
                    continue;
                }


                auto move = [](ops &currentop, unsigned int &val, int &maxval)
                {
                    switch (currentop)
                    {
                    case ops::INC:
                        if (val == maxval)
                            return false;
                        val++;
                        break;
                    case ops::DEC:
                        if (val == 0)
                            return false;
                        val--;
                        break;
                    }
                    return true;
                };

                auto canxmove = (!xatend) && (currentx != stopx);
                auto canymove = (!yatend) && (currenty != stopy);

                if (canxmove && canymove)
                {
                    auto movedice = this->ToolsObject.RndRange(1, 6) > 3 ? "MOVE_X" : "MOVE_Y";
                    if (movedice == "MOVE_X")
                    {
                        move(xop, currentx, maxx);
                    }
                    if (movedice == "MOVE_Y")
                    {
                        move(yop, currenty, maxy);
                    }
                }
                else
                {
                    if (canxmove)
                    {
                        move(xop, currentx, maxx);
                    }
                    if (canymove)
                    {
                        move(yop, currenty, maxy);
                    }
                }
            }
        }
    }
    return rval;
}
}
