//
// Created by eh on 07.01.2022.
//

#include "mapregions.hpp"
//
// Created by eh on 07.01.2022.
//

#ifndef DUNGONSOFMORGINAR_MAPREGIONS_HPP
#define DUNGONSOFMORGINAR_MAPREGIONS_HPP
namespace DofM
{
std::vector<Location::SLocation> MapObject::GetAllLocations()
{
    if (!IsCached)
    {
        IsCached = true;
        auto minx = this->StartLocation->X;
        auto miny = this->StartLocation->Y;
        auto maxx = this->StopLocation->X;
        auto maxy = this->StopLocation->Y;

        for(int ly = miny; ly <= maxy; ly++)
        {
            for(int lx = minx; lx <= maxx; lx++)
            {
                LocationCache.push_back(std::make_shared<Location>(lx, ly, 0));
            }
        }
    }
    return LocationCache;
}


MapRegions::MapRegions(std::string name, Location::SLocation start, Location::SLocation stop, Tile maintile) : StartLocation(start), StopLocation(stop)
{
    this->UniqueName = name;
    this->MainTile = maintile;
}

void MapRegions::AddWallsToRoom()
{
    MapObject n1;
    n1.StartLocation = std::make_shared<Location>(*StartLocation);
    n1.StopLocation = std::make_shared<Location>(StartLocation->X,StopLocation->Y,0);
    this->MapObjects.push_back(n1);

    MapObject n2;
    n2.StartLocation = std::make_shared<Location>(*StartLocation);
    n2.StopLocation = std::make_shared<Location>(StopLocation->X,StartLocation->Y,0);
    this->MapObjects.push_back(n2);

    MapObject n3;
    n3.StartLocation = std::make_shared<Location>(StartLocation->X,StopLocation->Y,0);
    n3.StopLocation = std::make_shared<Location>(*StopLocation);
    this->MapObjects.push_back(n3);

    MapObject n4;
    n4.StartLocation = std::make_shared<Location>(StopLocation->X,StartLocation->Y,0);
    n4.StopLocation = std::make_shared<Location>(*StopLocation);
    this->MapObjects.push_back(n4);

}

bool MapRegions::CanMove(Location::SLocation const& TargetLocation)
{
    // we are inside the walls, lets check if we are crashing with objects.
    for (auto &n : MapObjects)
    {
        for (auto &nn: n.GetAllLocations())
        {
            if (nn->IsSame(TargetLocation))
            {
                return false;
            }
        }
    }

    return true;
}

bool MapRegions::GetMoveCost(Location::SLocation const& TargetLocation)
{
    return MainTile.GetTileCost();
}


unsigned long MapRegions::Width()
{
    unsigned long rval = (StopLocation->X - StartLocation->X) + 1;
    return rval;
}

unsigned long MapRegions::Height()
{
    unsigned long rval = (StopLocation->Y - StartLocation->Y) + 1;
    return rval;
}



Location::SLocation MapRegions::GetCenter()
{
    unsigned long X = (StopLocation->X - StartLocation->X) / 2;
    unsigned long Y = (StopLocation->Y - StartLocation->Y) / 2;
    unsigned long Z = (StopLocation->Z - StartLocation->Z) / 2;
    return std::make_shared<Location>(X,Y,Z);
}

Location::SLocation MapRegions::GetRandomLocationInsideRoom()
{
    unsigned long X = StopLocation->X - (StopLocation->X - StartLocation->X);
    unsigned long Y = StopLocation->Y - (StopLocation->Y - StartLocation->Y);
    unsigned long Z = StopLocation->Z - (StopLocation->Z - StartLocation->Z);
    return std::make_shared<Location>(X,Y,Z);
}

void MapRegions::DrawMap(std::shared_ptr<NonBlockingTerminal> term, ScreenPos &drawstart)
{
    term->WriteToBuffer(fmt::format("Room: {}", this->UniqueName), ScreenPos(0,0).AddOffset(drawstart), {255,0,64,255});
    drawstart.IncreaseRow();
    term->WriteToBuffer(fmt::format("Description: {}", "Nothing right now"), ScreenPos(0,0).AddOffset(drawstart), {128,200,64,255});
    drawstart.IncreaseRow();
    drawstart.IncreaseRow();
    auto minx = this->StartLocation->X;
    auto miny = this->StartLocation->Y;
    auto maxx = this->StopLocation->X;
    auto maxy = this->StopLocation->Y;

    /// Clear the map area
    for(int ly = miny; ly <= maxy; ly++)
    {
        for(int lx = minx; lx <= maxx; lx++)
        {
            term->WriteToBuffer(" ", Location(lx, ly, 0).ReturnAsScreenPos().AddOffset(drawstart));
        }
    }
    for( auto  &n: MapObjects)
    {
        for (auto &nn: n.GetAllLocations())
        {
            term->WriteToBuffer(n.MapSymbol, nn->ReturnAsScreenPos().AddOffset(drawstart));
        }
        //
    }
}
}

#endif //DUNGONSOFMORGINAR_MAPREGIONS_HPP
