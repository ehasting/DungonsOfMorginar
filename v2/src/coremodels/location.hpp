//
// Created by eh on 1/6/22.
//

#ifndef DUNGONSOFMORGINAR_LOCATION_HPP
#define DUNGONSOFMORGINAR_LOCATION_HPP
#include "fmt/core.h"
#include <iostream>
namespace DofM
{
    class Location
    {
    public:
        std::string Label;
        long X;
        long Y;
        long Z;

        Location(std::string label, long x, long y, long z)
        {
            this->Label = label;
            this->X = x;
            this->Y = y;
            this->Z = z;
        }

        ~Location()
        {
            std::cout << fmt::format("~Location out of scope ({})", this->GetMatchLocation()) << std::endl;
        }

        bool operator==(const Location &a) const
        {
            return a.GetMatchLocation() == this->GetMatchLocation();
        }

        std::string_view GetMatchLocation() const
        {
            if (this->Label.empty())
            {
                return fmt::format("{}{}{}", this->X, this->Y, this->Z);
            } else
            {
                return fmt::format("{}{}{}{}", this->Label, this->X, this->Y, this->Z);
            }
        }

    };
}

#endif //DUNGONSOFMORGINAR_LOCATION_HPP
