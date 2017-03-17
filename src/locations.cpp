/*
MIT License

Copyright (c) 2017 Egil Hasting

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "locations.h"
#include "tools.h"
#include <iostream>
#include <vector>
#include "asciirenderengine.h"

Locations::Locations()
{
}


bool Locations::FindNewRoom(Direction::Directions dir)
{
    for (auto ex : this->CurrentRoom->Linked)
    {
        std::cout << "links" << std::endl;
        if ( dir == ex.first)
        {
            // std::cout << "Exit found!" << std::endl;
            if (this->CurrentRoom->IsDirectionBlocked(dir))
            {
                //std::cout << "Direction is blocked" << std::endl;
            }
            else
            {
                for (int roomcnt = 0; roomcnt < this->Map.size(); roomcnt++)
                {
                    std::cout << "maps" << std::endl;
                    if (this->Map.at(roomcnt).ID == ex.second->ID)
                    {
                        this->CurrentRoom->IsVisited = true;
                        this->CurrentRoom = &this->Map.at(roomcnt);
                        if (!this->Visited.count(this->CurrentRoom->ID))
                            this->Visited[this->CurrentRoom->ID] = true;
                        return true;
                    }
                }
                std::cout << "ERROR!" << std::endl;
                return false;
            }
        }
    }
    return false;
}
