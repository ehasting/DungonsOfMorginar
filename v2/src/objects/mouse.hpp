//
// Created by eh on 1/5/22.
//

#ifndef DUNGONSOFMORGINAR_MOUSE_HPP
#define DUNGONSOFMORGINAR_MOUSE_HPP
#include "coremodels/dynamicobject.hpp"
#include "coremodels/livingobject.hpp"
#include "coremodels/location.hpp"
#include "fmt/core.h"
#include <string>
#include <memory>
#include <iostream>

namespace DofM
{
    class Mouse : public LivingObject
    {
    public:
        inline static const std::string TypeName = "mouse";

        Mouse(std::string name)
            : LivingObject(name, TypeName, Location("mouse", 1, 1, 1))
        {
        }

        virtual bool Update(long tick)
        {
            Stats.Stamina.Current--;
            return true;
        };

        const std::string GetDescriptionLine()
        {
            return fmt::format("[{}] {} is having {} STAMINA and {} HP left", this->TypeName, UniqueName, Stats.Stamina.Current, Stats.Health.Current);
        }
    };
}

#endif //DUNGONSOFMORGINAR_MOUSE_HPP
