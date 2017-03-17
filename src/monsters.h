#ifndef MONSTERS_H
#define MONSTERS_H
#include "monster.h"

class Monsters
{
public:
    Monsters();
    std::vector<Monster> TheMonsters;
    void GenerateMonsters(int numbers);
    bool SetNextMonsterCoords(RoomCoords coords);
    std::vector<Monster> GetMonsters(RoomCoords coords)
    {
        std::vector<Monster> rval;
        for(auto n : this->TheMonsters)
        {
            if (n.Coords == coords)
                rval.push_back(n);

        }
        return rval;
    }

private:
    int interatorcounter = 0;
};

#endif // MONSTERS_H
