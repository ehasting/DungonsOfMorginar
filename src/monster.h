#ifndef MONSTER_H
#define MONSTER_H
#include "character.h"
#include "location.h"
#include "renderengine.h"
#include <string>

class Monster : public Character
{
public:
    Monster();
    RoomCoords Coords;
    RenderEngine::COLOR Color;
    std::string LastMessage;
    bool ShouldIMove();
    void MoveMosterRandom(std::vector<Location> Map);
    void SetMonsterLocation(RoomCoords loc)
    {
        this->Coords = RoomCoords(loc.X, loc.Y, loc.Z);
        std::cout << "Moster " << this->Coords.to_string() << std::endl;
        this->IsReady = true;
    }
    bool IsReady;
};

#endif // MONSTER_H
