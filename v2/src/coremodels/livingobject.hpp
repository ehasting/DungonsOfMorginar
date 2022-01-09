//
// Created by eh on 1/9/22.
//

#ifndef DUNGONSOFMORGINAR_LIVINGOBJECT_HPP
#define DUNGONSOFMORGINAR_LIVINGOBJECT_HPP

#include "location.hpp"
#include "dynamicobject.hpp"
#include "locatedobject.hpp"
#include "core/livingstats.hpp"
namespace DofM
{
    class LivingObject : public LocatedObject
    {
    public:
        std::string UniqueName;
        LivingStats Stats;
        virtual const std::string GetDescriptionLine() = 0;
    protected:
        LivingObject(std::string name, std::string tname, Location location)
            : LocatedObject(tname, location)
        {
            this->UniqueName = name;
            Stats.Stamina.Max = 100;
            Stats.Stamina.Current = 100;
        }
    };
}

#endif //DUNGONSOFMORGINAR_LIVINGOBJECT_HPP
