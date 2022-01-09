//
// Created by eh on 1/6/22.
//

#ifndef DUNGONSOFMORGINAR_LOCATEDOBJECT_HPP
#define DUNGONSOFMORGINAR_LOCATEDOBJECT_HPP
#include "location.hpp"
#include "dynamicobject.hpp"
namespace DofM
{
    class LocatedObject : public DynamicObject
    {
    protected:
        LocatedObject(std::string tname, Location location)
                : DynamicObject(tname)
        {
            this->ObjectLocation = std::make_shared<Location>(location);
        }

    public:
        virtual const std::string GetDescriptionLine() = 0;
        std::shared_ptr<Location> ObjectLocation;
    };
}

#endif //DUNGONSOFMORGINAR_LOCATEDOBJECT_HPP
