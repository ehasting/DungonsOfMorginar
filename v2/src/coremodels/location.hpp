//
// Created by eh on 1/6/22.
//

#ifndef DUNGONSOFMORGINAR_LOCATION_HPP
#define DUNGONSOFMORGINAR_LOCATION_HPP
#include "fmt/core.h"
#include <iostream>
#include <limits>
#include "core/screenpos.hpp"
namespace DofM
{
    class Location
    {
    public:
        typedef std::shared_ptr<Location> SLocation;
        unsigned long X;
        unsigned long Y;
        unsigned long Z;

        Location(Location const& loc)
        {
            this->X = loc.X;
            this->Y = loc.Y;
            this->Z = loc.Z;
        }
        Location(unsigned long x, unsigned long y, unsigned long z)
        {
            this->X = x;
            this->Y = y;
            this->Z = z;
        }

        ~Location()
        {
            //std::cout << fmt::format("~Location out of scope ({})", this->GetMatchLocation()) << std::endl;
        }

        void MoveNorth()
        {
            if (this->Y == 0)
            {
                return;
            }
            this->Y--;
            //std::cout << fmt::format("Y: {}", this->Y) << std::endl;
        }
        void MoveSouth()
        {
            if (this->Y == std::numeric_limits<unsigned long>::max())
            {
                return;
            }
            this->Y++;
            //std::cout << fmt::format("Y: {}", this->Y) << std::endl;
        }
        void MoveEast()
        {
            if (this->X == std::numeric_limits<unsigned long>::max())
            {
                return;
            }
            this->X++;
            //std::cout << fmt::format("X: {}", this->X) << std::endl;
        }
        void MoveWest()
        {
            if (this->X == 0)
            {
                return;
            }
            this->X--;
            //std::cout << fmt::format("X: {}", this->X) << std::endl;
        }

        bool IsBeside(const SLocation &other) const
        {
            int diffX = this->X - other->X;
            int diffY = this->Y - other->Y;
            auto eval = ( ((diffX == -1) || (diffX == 0) || (diffX == 1)) and ((diffY == -1) || (diffY == 0) || (diffY == 1)) );
            //std::cout << fmt::format("diffX: {}, diffY: {} == {}", diffX, diffY, eval) << std::endl;
            return eval;
        }

        std::string ToString()
        {
            return fmt::format("{},{},{}", X,Y,Z);
        }

        bool IsOntop(const SLocation &a) const
        {
            return a->GetMatchLocation() == this->GetMatchLocation();
        }

        std::string_view GetMatchLocation() const
        {
            return fmt::format("{}{}{}", this->X, this->Y, this->Z);
        }

        SLocation OffsetLocation(unsigned long x, unsigned long y, unsigned long z)
        {
            return std::make_shared<Location>(
                    this->X + x,
                    this->Y + y,
                    this->Z + z
            );
        }

        ScreenPos ReturnAsScreenPos(int offsetcol = 0, int offsetrow = 0)
        {
            int col = this->X + offsetcol;
            int row =  this->Y + offsetrow;
            return ScreenPos(col, row);
        }

    };
}

#endif //DUNGONSOFMORGINAR_LOCATION_HPP
