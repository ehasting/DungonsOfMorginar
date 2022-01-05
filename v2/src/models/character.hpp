//
// Created by eh on 1/5/22.
//

#ifndef DUNGONSOFMORGINAR_CHARACTER_H
#define DUNGONSOFMORGINAR_CHARACTER_H
#include "locatedobject.hpp"

class Character : public LocatedObject
{
public:
    inline static const std::string TypeName = "character";
    Character(Location loc) : LocatedObject(TypeName, loc)
    {
    }
    virtual bool Update(long tick)
    {
        std::cout << "Update from " << this->GetTypeName() << std::endl;
        std::cout << fmt::format("Location: {}", this->ObjectLocation->GetMatchLocation()) << std::endl;
        return true;
    };
    std::string Mastrubate()
    {
        return "I am mastrubating";
    }
};
/*
 * Room has objects
 * objects can be used for blocking in battle, and used by ai-controllerd chars in battle
 * objects can be picked up by characters and have properties
 */

#endif //DUNGONSOFMORGINAR_CHARACTER_H
