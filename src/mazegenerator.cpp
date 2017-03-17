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
#include "mazegenerator.h"
#include "location.h"
#include <random>
#include <vector>

MazeGenerator::MazeGenerator()
{
    this->RoomIdIndex = 0;


    int maxroom = 0;
    std::string thisroomid = this->GenerateRoomID(maxroom, (Direction::Directions)0, true);
    RoomCoords currentcoords(10,10,0);
    std::vector<RoomCoords> roomids;
    this->TheMaze[currentcoords.to_string()] = Location(thisroomid, "The sky is clear and this is room with id: " + thisroomid, currentcoords);
    while (true)
    {
        roomids.clear();
        for (int n : {0, 1, 2, 3})
        {
            if (Tools::getInstance().Dice(0, 20) > 14)
            {
                Direction::Directions movingto_dir = (Direction::Directions)n;
                std::string neighbourid = this->GenerateRoomID(maxroom, movingto_dir, true);
                std::cout << "Generate neigbour" << std::endl;

                RoomCoords neigbourcoords = this->TheMaze[currentcoords.to_string()].Coords.GetNeightbourRoomCoords(movingto_dir);
                if(!this->TheMaze.count(neigbourcoords.to_string()))
                {
                    this->TheMaze[neigbourcoords.to_string()] = Location(neighbourid, "Dark shadow lurks and this is room with id: " + neighbourid, neigbourcoords);
                }
                this->TheMaze[currentcoords.to_string()].Linked[movingto_dir] = &this->TheMaze[neigbourcoords.to_string()];
                this->TheMaze[neigbourcoords.to_string()].Linked[Direction::GetBackWay(movingto_dir)] = &this->TheMaze[currentcoords.to_string()];
                roomids.push_back(neigbourcoords);
            }
        }

        if (roomids.size() == 0)
        {
            Direction::Directions movingto_dir = (Direction::Directions)Tools::getInstance().Dice(0,3);

            std::string neighbourid = this->GenerateRoomID(maxroom, movingto_dir, true);
            RoomCoords neigbourcoords = this->TheMaze[currentcoords.to_string()].Coords.GetNeightbourRoomCoords(movingto_dir);
            std::cout << "Generate neigbour" << std::endl;
            if(!this->TheMaze.count(neigbourcoords.to_string()))
            {
                this->TheMaze[neigbourcoords.to_string()] = Location(neighbourid, "Misty forrest ahead and this is room with id: " + neighbourid, neigbourcoords);
            }            
            this->TheMaze[currentcoords.to_string()].Linked[movingto_dir] = &this->TheMaze[neigbourcoords.to_string()];
            this->TheMaze[neigbourcoords.to_string()].Linked[Direction::GetBackWay(movingto_dir)] = &this->TheMaze[currentcoords.to_string()];
            roomids.push_back(neigbourcoords);
        }

        int nextroomidindex = Tools::getInstance().Dice(0, roomids.size()-1);
        currentcoords = roomids.at(nextroomidindex);


        maxroom++;
        if (maxroom > 50)
        {
            this->TheMaze[currentcoords.to_string()].Description = "Finish!! you made it!";
            break;
        }
    }
    std::cout << "Mazegenerator Done" << std::endl;
}



void MazeGenerator::GenerateNeigbour(Location &source, Direction::Directions going, Location &neighbour)
{
    neighbour.Linked[Direction::GetBackWay(going)] = &source;
}

Direction::Directions MazeGenerator::GetNextWay(Direction::Directions commingfrom)
{
    while (true) {
        int retvalue = Tools::getInstance().Dice(0,3);
        if ((Direction::Directions)retvalue != commingfrom)
        {
            return (Direction::Directions)retvalue;
        }
    }
}
