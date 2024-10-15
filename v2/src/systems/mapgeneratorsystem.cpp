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
    int tiles = this->ToolsObject.RndRange(12,24);
    if (( (tiles+exits.size()) % 2) != 0)
        tiles++;
    std::vector<Location::SLocation> starttiles;
    for(auto &n : exits)
    {
        auto t1 = std::make_shared<MapObject>();
        t1->Point = std::make_shared<Location>(n->X,n->Y,0);
        t1->Tile.TileType = Tile::TileTypes::DOOR;
        t1->MapSymbol = "¤";
        this->NonDuplicateVectorPush(t1, rval);
        starttiles.push_back(t1->Point);
        if (maxx < n->X)
        {
            maxx = n->X;
        }
        if (maxy < n->Y)
        {
            maxy = n->Y;
        }
    }
    for (int x  = 0; x < tiles; x++)
    {
        int tilesx = this->ToolsObject.RndRange(1,maxx);
        int tilesy = this->ToolsObject.RndRange(1,maxy);
        auto t1 = std::make_shared<MapObject>();
        t1->Point = std::make_shared<Location>(tilesx,tilesy,00);
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
        unsigned int startx = (*it)->X;
        unsigned int starty = (*it)->Y;
        it++;
        auto secondloc = (*it);
        unsigned int stopx = (*it)->X;
        unsigned int stopy = (*it)->Y;
        it++;

        fmt::print("{} -> {}\n", firstloc->ToString(), secondloc->ToString());
        xop = ops::INC, yop = ops::INC;
        if (startx > stopx)
        {
            xop = ops::DEC;
        }
        if (starty > stopy)
        {
            yop = ops::DEC;
        }
        int retrycount = 0;
        while(!foundwall)
        {

            //fmt::print("Retry count: {}\n", retrycount);
            retrycount = 0;
            auto nextloc = std::make_shared<Location>(startx,stopy,00);
            if (nextloc->IsSame(secondloc))
            {
                fmt::print("Found second loc\n");
                foundwall = true;
                if (this->ToolsObject.RndRange(0,24) > 12)
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
                    fmt::print("Generate room on {} x {} -> {} x {}\n",nextloc->X, nextloc->Y, roomendloc->X, roomendloc->Y);
                    this->GenerateRoom(nextloc, roomendloc, rval);
                }
                continue;
            }
            else
            {
                auto t1 = std::make_shared<MapObject>();
                t1->Point = std::make_shared<Location>(startx,starty,00);
                fmt::print("{} ({}) x={}, y={}\n", t1->Point->ToString(), secondloc->ToString(), ops_tostring(xop), ops_tostring(yop));
                t1->Tile.TileType = Tile::TileTypes::STONE_INCAVE_GROUND;
                // t1.MapSymbol = "X";
                this->NonDuplicateVectorPush(t1, rval);

            retry_move:
                retrycount++;
                bool nextmove = this->ToolsObject.RndRange(0,24) > 12;
                bool xatend = false;
                bool yatend = false;
                if (startx == stopx)
                {
                    xatend = true;
                }
                if (starty == stopy)
                {
                    yatend = true;
                }

                if (xatend && yatend)
                {
                    foundwall = true;
                    continue;
                }

                if (nextmove && !xatend)
                {
                    if (startx != stopx)
                    {
                        switch (xop)
                        {
                        case ops::INC:
                            if (startx == maxx)
                                goto retry_move;
                            startx++;
                            break;
                        case ops::DEC:
                            if (startx == 0)
                                goto retry_move;
                            startx--;
                            break;
                        }
                    }
                }
                else if (!yatend)
                {
                    if (starty != stopy)
                    {
                        switch (yop)
                        {
                        case ops::INC:
                            if (starty == maxy)
                                goto retry_move;
                            starty++;
                            break;
                        case ops::DEC:
                            if (starty == 0)
                                goto retry_move;
                            starty--;
                            break;
                        }
                    }
                }
            }
        }
    }
    return rval;
}
}
