//
// Created by eh on 07.01.2022.
//

#ifndef DUNGONSOFMORGINAR_MAPREGIONS_HPP
#define DUNGONSOFMORGINAR_MAPREGIONS_HPP
#include <string_view>
#include <iostream>
#include "coremodels/location.hpp"
namespace DofM
{
    class MapRegions
    {
    public:
        typedef std::shared_ptr<MapRegions> SMapRegions;
        std::string UniqueName;
        Location::SLocation StartLocation;
        Location::SLocation StopLocation;
        MapRegions(std::string name, Location::SLocation start, Location::SLocation stop) : StartLocation(start), StopLocation(stop)
        {
            this->UniqueName = name;
        }

        bool CanMove(Location::SLocation const& TargetLocation) const
        {
            if ((StartLocation->Y < TargetLocation->Y &&
                 StopLocation->Y > TargetLocation->Y) &&
                (StartLocation->X < TargetLocation->X &&
                 StopLocation->X > TargetLocation->X))
            {
                return true;
            }
            return false;
        }

        // Wall in room is taking one space - so we check if we are at wall, not at edge of room.
        bool IsAtNorthWall(Location::SLocation const& TargetLocation)
        {
            return this->StartLocation->Y + 1 == TargetLocation->Y;
        }
        bool IsAtSouthWall(Location::SLocation const& TargetLocation)
        {
            return this->StopLocation->Y - 1 == TargetLocation->Y;
        }
        bool IsAtWestWall(Location::SLocation const& TargetLocation)
        {
            return this->StartLocation->X + 1 == TargetLocation->X;
        }
        bool IsAtEastWall(Location::SLocation const& TargetLocation)
        {
            //std::cout << fmt::format("{} == {}", this->StopLocation->X - 1,  TargetLocation->X) << std::endl;
            return this->StopLocation->X - 1 == TargetLocation->X;
        }

        unsigned long Width()
        {
            unsigned long rval = (StopLocation->X - StartLocation->X) + 1;
            return rval;
        }

        unsigned long Height()
        {
            unsigned long rval = (StopLocation->Y - StartLocation->Y) + 1;
            return rval;
        }



        Location::SLocation GetCenter()
        {
            unsigned long X = (StopLocation->X - StartLocation->X) / 2;
            unsigned long Y = (StopLocation->Y - StartLocation->Y) / 2;
            unsigned long Z = (StopLocation->Z - StartLocation->Z) / 2;
            return std::make_shared<Location>(X,Y,Z);
        }

        Location::SLocation GetRandomLocationInsideRoom()
        {
            unsigned long X = StopLocation->X - (StopLocation->X - StartLocation->X);
            unsigned long Y = StopLocation->Y - (StopLocation->Y - StartLocation->Y);
            unsigned long Z = StopLocation->Z - (StopLocation->Z - StartLocation->Z);
            return std::make_shared<Location>(X,Y,Z);
        }


        constexpr static std::string_view Region1 {
                            "********"
                            "***  ***"
                            "**   ***"
                            "**   ***"
                            "**   ***"
                            "****  **"
                            "*****   "
                            "********"};
        void DrawRegion(std::string_view tile)
        {
            for (int x = 0; x < tile.size(); x++)
            {
                std::cout << tile[x];
                if (x != 0)
                {
                    if ((x % 8) == 0)
                        std::cout << std::endl;
                }
            }
        }

    };
}

#endif //DUNGONSOFMORGINAR_MAPREGIONS_HPP
