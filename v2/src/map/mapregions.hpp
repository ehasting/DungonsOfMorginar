//
// Created by eh on 07.01.2022.
//

#ifndef DUNGONSOFMORGINAR_MAPREGIONS_HPP
#define DUNGONSOFMORGINAR_MAPREGIONS_HPP
#include <string_view>
#include <iostream>
#include "coremodels/location.hpp"
#include "core/nonblockingterminal.hpp"
#include "map/tile.hpp"
#include "tools.hpp"
namespace DofM
{
    class MapObject
    {
    private:
        std::vector<Location::SLocation> LocationCache;
        bool IsCached = false;
    public:
        Location::SLocation StartLocation;
        Location::SLocation StopLocation;
        std::string MapSymbol = "░";
        DofM::Tile Tile;
        std::vector<Location::SLocation> GetAllLocations();
    };
    class MapRegions
    {
        DofM::Tools ToolsObject;
    public:
        typedef std::shared_ptr<MapRegions> SMapRegions;
        std::string UniqueName;
        std::string Description;
        Location::SLocation StartLocation;
        Location::SLocation StopLocation;
        Tile MainTile;
        std::vector<MapObject> MapObjects;

        MapRegions(std::string name, Location::SLocation start, Location::SLocation stop, Tile maintile);

        void AddWallsToRoom();
        bool CanMove(Location::SLocation const& TargetLocation);
        bool GetMoveCost(Location::SLocation const& TargetLocation);
        unsigned long Width();
        unsigned long Height();
        Location::SLocation GetCenter();
        Location::SLocation GetRandomLocationInsideRoom();
        void DrawMap(std::shared_ptr<NonBlockingTerminal> term, ScreenPos &drawstart);
    };
}

#endif //DUNGONSOFMORGINAR_MAPREGIONS_HPP
