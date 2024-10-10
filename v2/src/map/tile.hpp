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
            WALL,
            GRASS_OUTSIDE_GROUND,
            GRAVEL_OUTSIDE_GROUND,
            WATER_OUTSIDE_GROUND,
            STONE_INCAVE_GROUND,
            CARPET_INHOUSE_FLOOR,
            WOOD_INHOUSE_FLOOR,
        }
        TileType = TileTypes::GRASS_OUTSIDE_GROUND;

        enum MovementSpeeds
        {
            FAST,
            NORMAL,
            SLOW,
            SWIMMING,
            BLOCKED
        };

        // Calculate stamina usage based on ground type -retract form stamina pool from all living
        // at 0-ish they need to sleep, ai and human
        MovementSpeeds GetTileCost()
        {
            switch (this->TileType)
            {
                case GRASS_OUTSIDE_GROUND:
                return MovementSpeeds::NORMAL;
                    break;
                case GRAVEL_OUTSIDE_GROUND:
                    return MovementSpeeds::NORMAL;
                    break;
                case WATER_OUTSIDE_GROUND:
                    return MovementSpeeds::SWIMMING;
                    break;
                case STONE_INCAVE_GROUND:
                case CARPET_INHOUSE_FLOOR:
                case WOOD_INHOUSE_FLOOR:
                    return MovementSpeeds::FAST;
                    break;
                case WALL:
                    return MovementSpeeds::BLOCKED;
            }
        }

    };
}

#endif //DUNGONSOFMORGINAR_TILE_HPP
