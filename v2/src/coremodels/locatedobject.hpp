//
// Created by eh on 1/6/22.
//

#ifndef DUNGONSOFMORGINAR_LOCATEDOBJECT_HPP
#define DUNGONSOFMORGINAR_LOCATEDOBJECT_HPP
#include "location.hpp"
#include "dynamicobject.hpp"
#include "map/mapregions.hpp"
namespace DofM
{
    class LocatedObject : public DynamicObject
    {
    protected:
        LocatedObject(std::string name, std::string tname, Location::SLocation location, DynamicObjectList dynobj)
                : DynamicObject(name, tname, dynobj)
        {
            this->ObjectLocation = location;
        }

    public:
        typedef std::shared_ptr<LocatedObject> SLocatedObject;
        virtual const std::string GetDescriptionLine() = 0;
        Location::SLocation ObjectLocation;
        MapRegions::SMapRegions ObjectMapRegion;
        SDynamicObject GetLocatedObject()
        {
            return SLocatedObject(this);
            //return std::dynamic_pointer_cast<T>(std::make_shared<T>(this));
        }
        void SetMap(MapRegions::SMapRegions map, bool placerandom = true)
        {
            this->ObjectMapRegion = map;
            //fmt::print("[{}] Ref cnt. map {}\n", this->UniqueName, map.use_count());
            if (placerandom)
            {
                // Copy object!! its returning a shared pointer - or we will start "moving" the tile
                this->ObjectLocation = std::make_shared<Location>(*map->GetRandomLocationInsideRoom());
                //fmt::print("Found {} location: {}\n",this->UniqueName, this->ObjectLocation->ToString());
            }
            else
            {
                this->ObjectLocation = map->GetCenter();
            }
        }
    };
}

#endif //DUNGONSOFMORGINAR_LOCATEDOBJECT_HPP
