#include "monsters.h"
#include "monster.h"

Monsters::Monsters()
{

}

void Monsters::GenerateMonsters(int numbers)
{
    for(int x = 0; x < numbers; x++)
    {
        Monster tmp;
        tmp.GenerateCharacter();
        this->TheMonsters.push_back(tmp);
    }
}

bool Monsters::SetNextMonsterCoords(RoomCoords coords)
{
    if ( (this->interatorcounter+1) < this->TheMonsters.size() )
    {
        this->TheMonsters.at(this->interatorcounter).SetMonsterLocation(coords);
        this->interatorcounter++;
        return true;
    }
    return false;
}

