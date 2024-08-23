//
// Created by eh on 1/5/22.
//

#ifndef DUNGONSOFMORGINAR_CHARACTER_H
#define DUNGONSOFMORGINAR_CHARACTER_H
#include "coremodels/dynamicobject.hpp"
#include "coremodels/livingobject.hpp"
#include "coremodels/location.hpp"


namespace DofM
{
    class Character : public LivingObject
    {
    public:
        inline static const std::string TypeName = "character";
        std::string CurrentDescription;
        Character(std::string name, Location::SLocation loc, DynamicObjectList dynobj)
            : LivingObject(name, TypeName, loc, dynobj)
        {
        }

        virtual bool Update(long long int tick)
        {
            //std::cout << "Update from " << this->GetTypeName() << std::endl;
            //std::cout << fmt::format("Location: {}", this->ObjectLocation->GetMatchLocation()) << std::endl;
            this->CurrentDescription = fmt::format("[{}] {} is having {} STAMINA and {} HP left (Loc: {})", this->TypeName, UniqueName, Stats.Stamina.GetCurrent(), Stats.Health.GetCurrent(), this->ObjectLocation->ToString());
            return true;
        };

        const std::string GetDescriptionLine()
        {
            return this->CurrentDescription;
        }
    };
}
/*
 * Room has objects
 * objects can be used for blocking in battle, and used by ai-controllerd chars in battle
 * objects can be picked up by characters and have properties
 */

#endif //DUNGONSOFMORGINAR_CHARACTER_H
