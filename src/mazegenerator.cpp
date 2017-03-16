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
    this->directions[0] = "north";
    this->directions[1] = "south";
    this->directions[2] = "east";
    this->directions[3] = "west";


    int maxroom = 0;
    std::string fromdirection = this->directions[0];
    std::string thisroomid = this->GenerateRoomID(maxroom, "blackhole", fromdirection, true);
    std::string commingfrom = "blackhole";
    RoomCoords currentcoords(10,10,0);
    RoomCoords n(currentcoords.to_string());
    std::vector<std::string> roomids;
    this->TheMaze[currentcoords.to_string()] = Location(thisroomid, "this is room with id: " + thisroomid, currentcoords);
    while (true)
    {
        roomids.clear();


        for(auto n : this->directions)
        {
            if (this->Dice(0,20) > 14)
            {
                std::string neighbourid = this->GenerateRoomID(maxroom, thisroomid, n.second, true);
                std::cout << "Generate neigbour" << std::endl;

                RoomCoords neigbourcoords = this->TheMaze[currentcoords.to_string()].GetNeightbourRoom(n.second);
                if(this->TheMaze.count(neigbourcoords.to_string()))
                {
                    this->TheMaze[currentcoords.to_string()].Linked[n.second] = &this->TheMaze[neigbourcoords.to_string()];
                    this->TheMaze[neigbourcoords.to_string()].Linked[this->GetBackWay(n.second)] = &this->TheMaze[currentcoords.to_string()];
                }
                else {
                    this->TheMaze[neigbourcoords.to_string()] = Location(neighbourid, "this is room with id: " + neighbourid, this->TheMaze[currentcoords.to_string()].GetNeightbourRoom(n.second));
                    this->TheMaze[currentcoords.to_string()].Linked[n.second] = &this->TheMaze[neigbourcoords.to_string()];
                    this->TheMaze[neigbourcoords.to_string()].Linked[this->GetBackWay(n.second)] = &this->TheMaze[currentcoords.to_string()];
                }
                roomids.push_back(neigbourcoords.to_string());
            }
        }

        if (roomids.size() == 0)
        {
            std::string defaultdirection = "north";
            std::string neighbourid = this->GenerateRoomID(maxroom, thisroomid, defaultdirection, true);
            RoomCoords neigbourcoords = this->TheMaze[currentcoords.to_string()].GetNeightbourRoom(defaultdirection);
            std::cout << "Generate neigbour" << std::endl;
            if(this->TheMaze.count(neigbourcoords.to_string()))
            {
                this->TheMaze[currentcoords.to_string()].Linked[defaultdirection] = &this->TheMaze[neigbourcoords.to_string()];
                this->TheMaze[neigbourcoords.to_string()].Linked[this->GetBackWay(defaultdirection)] = &this->TheMaze[currentcoords.to_string()];
            }
            else
            {

            }

            this->TheMaze[neigbourcoords.to_string()] = Location(neighbourid, "this is room with id: " + neighbourid, this->TheMaze[currentcoords.to_string()].GetNeightbourRoom(defaultdirection));
            this->TheMaze[currentcoords.to_string()].Linked[defaultdirection] = &this->TheMaze[neigbourcoords.to_string()];
            this->TheMaze[neigbourcoords.to_string()].Linked[this->GetBackWay(defaultdirection)] = &this->TheMaze[currentcoords.to_string()];
            roomids.push_back(neigbourcoords.to_string());
        }

        int nextroomidindex = this->Dice(0, roomids.size()-1);
        commingfrom = thisroomid;
        currentcoords = roomids.at(nextroomidindex);
        thisroomid = this->TheMaze[currentcoords.to_string()].ID;


        maxroom++;
        if (maxroom > 50)
        {
            this->TheMaze[currentcoords.to_string()].Description = "Finish!! you made it!";
            break;
        }
    }
    std::cout << "Mazegenerator Done" << std::endl;
}



void MazeGenerator::GenerateNeigbour(Location &source, std::string going, Location &neighbour)
{
    neighbour.Linked[this->GetBackWay(going)] = &source;
}

int MazeGenerator::Dice(int min, int max)
{
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
    return dist6(this->rng);
}

std::string MazeGenerator::GetNextWay(std::string commingfrom)
{
    std::string rval;


    while (true) {
        int retvalue = this->Dice(0,3);
        if (this->directions[retvalue] != commingfrom)
        {
            return this->directions[retvalue];
        }
    }
}
