//
// Created by eh on 07.01.2022.
//

#ifndef DUNGONSOFMORGINAR_MAPREGIONS_HPP
#define DUNGONSOFMORGINAR_MAPREGIONS_HPP
#include <string_view>
#include <iostream>

namespace DofM
{
    class MapRegions
    {
    public:
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
