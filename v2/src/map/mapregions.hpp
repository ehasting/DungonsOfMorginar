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
#include "mapobject.hpp"
#include "systems/mapgeneratorsystem.hpp"
namespace DofM
{
    class MapRegions
    {
        DofM::Tools ToolsObject;
        std::vector<MapObject::SMapObject> TileList;
        MapGeneratorSystem MapGenerator;
    public:
        typedef std::shared_ptr<MapRegions> SMapRegions;
        std::string UniqueName;
        std::string Description;

        MapRegions(std::string name, std::vector<Location::SLocation> exits);

        void AddWallsToRoom();
        bool CanMove(Location::SLocation const& TargetLocation);
        int GetMoveCost(Location::SLocation const& TargetLocation);
        unsigned long Width();
        unsigned long Height();
        Location::SLocation GetCenter();
        Location::SLocation GetRandomLocationInsideRoom();
        void DrawMap(std::shared_ptr<NonBlockingTerminal> term, ScreenPos &drawstart);
    };
}

#endif //DUNGONSOFMORGINAR_MAPREGIONS_HPP
