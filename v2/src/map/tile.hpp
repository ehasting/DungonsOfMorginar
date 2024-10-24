//
// Created by eh on 06.01.2022.
//

#ifndef DUNGONSOFMORGINAR_TILE_HPP
#define DUNGONSOFMORGINAR_TILE_HPP
#include <string>

namespace DofM
{
    class Tile
    {
    public:
        enum class MovementSpeeds
        {
            FAST,
            NORMAL,
            SLOW,
            SWIMMING,
            BLOCKED
        };
        enum class TileTypes
        {
            EMPTY,
            WALL,
            GRASS_OUTSIDE_GROUND,
            GRAVEL_OUTSIDE_GROUND,
            WATER_OUTSIDE_GROUND,
            STONE_INCAVE_GROUND,
            CARPET_INHOUSE_FLOOR,
            WOOD_INHOUSE_FLOOR,
            DOOR,
        };
        TileTypes TileType = TileTypes::GRASS_OUTSIDE_GROUND;
        MovementSpeeds MovementSpeed = MovementSpeeds::NORMAL;
        Tile() { }
        Tile(Tile::TileTypes type, Tile::MovementSpeeds speed)
            : TileType(type), MovementSpeed(speed)
        {

        }
        std::string GetTileTypeLetter()
        {
            std::string xyz = "abcdefghijklmnopqrstuvwxyz";
            return xyz.substr((int)this->TileType, 1);
        }
        // Calculate stamina usage based on ground type -retract form stamina pool from all living
        // at 0-ish they need to sleep, ai and human
        MovementSpeeds GetTileCost()
        {
            switch (this->TileType)
            {
            case TileTypes::GRASS_OUTSIDE_GROUND:
                return MovementSpeeds::NORMAL;
                    break;
                case TileTypes::GRAVEL_OUTSIDE_GROUND:
                    return MovementSpeeds::NORMAL;
                    break;
                case TileTypes::WATER_OUTSIDE_GROUND:
                    return MovementSpeeds::SWIMMING;
                    break;
                case TileTypes::STONE_INCAVE_GROUND:
                case TileTypes::CARPET_INHOUSE_FLOOR:
                case TileTypes::WOOD_INHOUSE_FLOOR:
                    return MovementSpeeds::FAST;
                    break;
                case TileTypes::WALL:
                    return MovementSpeeds::BLOCKED;
            }
        }

    };
}

#endif //DUNGONSOFMORGINAR_TILE_HPP
