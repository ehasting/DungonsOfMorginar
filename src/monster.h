#ifndef MONSTER_H
#define MONSTER_H
#include "character.h"
#include "location.h"

class Monster : public Character
{
public:
    Monster();
    RoomCoords Coords;
    bool ShouldIMove();
    void MoveMosterRandom();
    void SetMonsterLocation(RoomCoords loc)
    {
        this->Coords = loc;
    }
};

#endif // MONSTER_H
