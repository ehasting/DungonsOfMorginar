#ifndef MONSTERS_H
#define MONSTERS_H
#include "monster.h"

class Monsters
{
public:
    Monsters(int numOfMonsters);
    std::vector<Monster> TheMonsters;

    bool GenerateNextMonster(RoomCoords coords);
    std::vector<Monster*> GetMonsters(RoomCoords coords)
    {
        std::vector<Monster*> rval;
        for(int x = 0; x < this->TheMonsters.size(); x++)
        {
            if (this->TheMonsters[x].Coords == coords)
            {
                rval.push_back(&this->TheMonsters[x]);
            }

        }
        return rval;
    }

private:
    int MonsterCount = 0;
};

#endif // MONSTERS_H
