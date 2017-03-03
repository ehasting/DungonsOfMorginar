#include "locations.h"
#include "tools.h"
#include <iostream>
#include <vector>
#include "asciirenderengine.h"

Locations::Locations()
{
    this->CommandIsKnown = true;
}


void Locations::FindNewRoom(std::string input)
{
    this->CommandIsKnown = false;
    for (auto ex : this->CurrentRoom->Linked)
    {
        if (input == Tools::toLowercase(ex.first))
        {
            // std::cout << "Exit found!" << std::endl;
            if (this->CurrentRoom->IsDirectionBlocked(input))
            {
                //std::cout << "Direction is blocked" << std::endl;
            }
            else
            {
                this->CommandIsKnown = true;
                for (int roomcnt = 0; roomcnt < this->Map.size(); roomcnt++)
                {
                    if (this->Map.at(roomcnt).ID == ex.second->ID)
                    {
                        this->CurrentRoom = &this->Map.at(roomcnt);
                        return;
                    }
                }
                std::cout << "ERROR!" << std::endl;
                return;
            }
        }
    }
}
