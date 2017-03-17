#include "monster.h"
#include "tools.h"
#include "location.h"
#include "renderengine.h"

Monster::Monster() : Character()
{
    this->Coords = RoomCoords(0,0,0);
    this->Name = "Dragon";
    this->LastMessage = "";
    this->Color = (RenderEngine::COLOR)Tools::getInstance().Dice(0, 4);
    this->IsReady = true;
}

bool Monster::ShouldIMove()
{
    if (Tools::getInstance().Dice(1,20) > 3)
        return true;
    return false;
}

void Monster::MoveMosterRandom(std::vector<Location> &Map)
{
    int dice;
    Direction::Directions direction;

    while (true) {
        dice = Tools::getInstance().Dice(0,3);
        direction = (Direction::Directions)dice;
        std::cout << this->Coords.to_string() << std::endl;
        RoomCoords trymoveto = this->Coords.GetNeightbourRoomCoords(direction);
        bool found = false;
        for( Location n : Map )
        {
            //std::cout << "n.Coords" << n.Coords.to_string() << " :: "<< trymoveto.to_string() << std::endl;
            if (n.Coords == trymoveto)
            {
                found = true;
                break;
            }
        }
        std::cout << "move" << std::endl;
        if (!found)
            continue;
        if (Tools::getInstance().Dice(0,6) > 3)
           break;
    }
    this->Moves++;
    this->LastMessage = "Moved from " + this->Coords.to_string();
    this->Coords = this->Coords.GetNeightbourRoomCoords(direction);

}
