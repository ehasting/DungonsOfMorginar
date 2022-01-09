//
// Created by eh on 1/5/22.
//

#ifndef DUNGONSOFMORGINAR_CHARACTER_H
#define DUNGONSOFMORGINAR_CHARACTER_H
#include "coremodels/locatedobject.hpp"
#include "coremodels/livingobject.hpp"
#include "coremodels/location.hpp"
namespace DofM
{
    class Character : public LivingObject
    {
    public:
        inline static const std::string TypeName = "character";

        Character(std::string name, Location loc)
            : LivingObject(name, TypeName, loc)
        {
        }

        virtual bool Update(long tick)
        {
            //std::cout << "Update from " << this->GetTypeName() << std::endl;
            //std::cout << fmt::format("Location: {}", this->ObjectLocation->GetMatchLocation()) << std::endl;
            Stats.Stamina.Current--;
            return true;
        };

        const std::string GetDescriptionLine()
        {
            return fmt::format("[{}] {} is having {} STAMINA and {} HP left",this->TypeName, UniqueName, Stats.Stamina.Current, Stats.Health.Current);
        }
    };
}
/*
 * Room has objects
 * objects can be used for blocking in battle, and used by ai-controllerd chars in battle
 * objects can be picked up by characters and have properties
 */

#endif //DUNGONSOFMORGINAR_CHARACTER_H
