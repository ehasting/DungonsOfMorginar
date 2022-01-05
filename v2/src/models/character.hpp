//
// Created by eh on 1/5/22.
//

#ifndef DUNGONSOFMORGINAR_CHARACTER_H
#define DUNGONSOFMORGINAR_CHARACTER_H
#include "dynamicobject.hpp"

class Character : DynamicObject
{
    /*
     * Room has objects
     * objects can be used for blocking in battle, and used by ai-controllerd chars in battle
     * objects can be picked up by characters and have properties
     */
    virtual bool Update(long tick)
    {
        return true;
    };
};


#endif //DUNGONSOFMORGINAR_CHARACTER_H
