#include "monsters.h"
#include "monster.h"

Monsters::Monsters(int numberOfMonsters)
{
    this->TheMonsters.clear();
    this->MonsterCount = numberOfMonsters;
}


bool Monsters::GenerateNextMonster(RoomCoords coords)
{
    if (this->TheMonsters.size() <= this->MonsterCount)
    {
        Monster newmonster;
        newmonster.GenerateCharacter();
        newmonster.SetMonsterLocation(coords);
        newmonster.Name = "Dragon Monster " + std::to_string(this->TheMonsters.size());
        this->TheMonsters.push_back(newmonster);
        return true;
    }
    return false;
}

