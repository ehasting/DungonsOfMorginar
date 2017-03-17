#include "monsters.h"
#include "monster.h"

Monsters::Monsters()
{

}

void Monsters::GenerateMonsters(int numbers)
{
    this->TheMonsters.clear();
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
        std::cout << "Moster " << this->interatorcounter << " placed: " << coords.to_string() << std::endl;
        this->TheMonsters.at(this->interatorcounter).SetMonsterLocation(coords);
        this->interatorcounter++;
        return true;
    }
    return false;
}

