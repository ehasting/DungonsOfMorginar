//
// Created by eh on 06.01.2022.
//

#ifndef DUNGONSOFMORGINAR_TILE_HPP
#define DUNGONSOFMORGINAR_TILE_HPP

namespace DofM
{
    class Tile
    {
    public:
        enum TileTypes
        {
            BLOCKED,
            GRASS_OUTSIDE_GROUND,
            GRAVEL_OUTSIDE_GROUND,
            WATER_OUTSIDE_GROUND,

            STONE_INCAVE_GROUND,

            CARPET_INHOUSE_FLOOR,
            WOOD_INHOUSE_FLOOR,
        };
        // Calculate stamina usage based on ground type -retract form stamina pool from all living
        // at 0-ish they need to sleep, ai and human
        TileTypes TileType = TileTypes::GRASS_OUTSIDE_GROUND;
        static inline int GetTileCost(TileTypes type)
        {
            switch (type)
            {
                case GRASS_OUTSIDE_GROUND:
                    return 2;
                    break;
                case GRAVEL_OUTSIDE_GROUND:
                    return 2;
                    break;
                case WATER_OUTSIDE_GROUND:
                    return 10;
                    break;
                case STONE_INCAVE_GROUND:
                case CARPET_INHOUSE_FLOOR:
                case WOOD_INHOUSE_FLOOR:
                    return 1;
                    break;
            }
            return 0;
        }

    };
}

#endif //DUNGONSOFMORGINAR_TILE_HPP
