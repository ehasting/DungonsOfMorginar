#include "monster.h"
#include "tools.h"
#include "location.h"

Monster::Monster() : Character()
{
    this->Name = "Unamed Monster";
}

bool Monster::ShouldIMove()
{
    if (Tools::Dice(1,20) > 12)
        return true;
    return false;
}

void Monster::MoveMosterRandom(std::vector<Location> &Map)
{
    int dice;
    std::string direction;
    while (true) {
        dice = Tools::Dice(0,3);
        direction = Location::DIRECTIONS[dice];
        bool found = false;
        for( Location n : Map )
        {
            if (n.Coords.to_string() == this->Coords.GetNeightbourRoom(direction).to_string())
            {
                found = true;
                break;
            }
        }
        if (!found)
            continue;
        if (Tools::Dice(0,6) > 3)
           break;
    }
    this->Moves++;
    this->Coords = this->Coords.GetNeightbourRoom(direction);

}
