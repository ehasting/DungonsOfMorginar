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

std::vector<std::string> path;
int itercount = 0;

MazeGenerator::MazeGenerator()
{
    this->RoomIdIndex = 0;

    for(int x = 0; x < 4; x++)
    { // int id, std::string name, std::string placement, std::string desc)
        Item n(x, "key" + std::to_string(x), "on the table", "rusty");
        this->Items.push_back(n);
    }

    int maxroom = 0;
    std::string thisroomid = this->GenerateRoomID(maxroom, (Direction::Directions)0, true);
    RoomCoords currentcoords(10,10,0);
    std::vector<RoomCoords> roomids;
    this->TheMaze[currentcoords.to_string()] = Location(thisroomid, "The sky is clear and this is room with id: " + thisroomid, currentcoords);
    int placeditems = 0;
    while (true)
    {
        roomids.clear();
        for (int n : {0, 1, 2, 3})
        {
            if (Tools::getInstance().Dice(0, 20) > 14)
            {
                if ( (Tools::getInstance().Dice(0,20) > 10) && placeditems <this->Items.size() )
                {
                    this->TheMaze[currentcoords.to_string()].ItemList.push_back(&this->Items[placeditems]);
                    placeditems++;
                }
                Direction::Directions movingto_dir = (Direction::Directions)n;
                std::string neighbourid = this->GenerateRoomID(maxroom, movingto_dir, true);
                std::cout << "Generate neigbour" << std::endl;

                RoomCoords neigbourcoords = this->TheMaze[currentcoords.to_string()].Coords.GetNeightbourRoomCoords(movingto_dir);
                if(!this->TheMaze.count(neigbourcoords.to_string()))
                {
                    this->TheMaze[neigbourcoords.to_string()] = Location(neighbourid, "Dark shadow lurks and this is room with id: " + neighbourid, neigbourcoords);
                }
                if ( (Tools::getInstance().Dice(0,20) > 10) )
                {
                    int rndindex = Tools::getInstance().Dice(0, placeditems);
                    this->TheMaze[currentcoords.to_string()].Blockers[movingto_dir] = &this->Items[rndindex];
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

    RoomCoords startcoords(10,10,0);
    this->WalkTheLabyrinth(&this->TheMaze[startcoords.to_string()], &this->TheMaze[startcoords.to_string()], currentcoords);
    for (std::string n : path)
    {
        std::cout << n << " -> ";
    }
    std::cout << "Mazegenerator Done" << std::endl;
}
bool exitfound = false;
void MazeGenerator::WalkTheLabyrinth(Location *DestinationRoom, Location *SourceRoom, RoomCoords &Target)
{
    itercount++;
    std::cout << DestinationRoom->Coords.to_string() << " vs " << Target.to_string() << std::endl;

    for (std::string n : path)
    {
        if (n == DestinationRoom->Coords.to_string())
        {
            return;
        }
    }

    if (DestinationRoom->Coords.to_string() == Target.to_string())
    {
        path.push_back("Out");
        exitfound = true;
        return;
    }

    Direction::Directions dir;
    std::cout << itercount << " - Walking " << DestinationRoom->Coords.to_string() <<std::endl;
    for (int n : {0, 1, 2, 3})
    {
        if (exitfound)
            return;
        dir = (Direction::Directions)n;

        if (DestinationRoom->Linked.count(dir))
        {
            if (DestinationRoom->Linked[dir]->Coords.to_string() == SourceRoom->Coords.to_string())
            {
                continue;
            }
            std::cout << Direction::to_string(dir) <<std::endl;
            path.push_back(DestinationRoom->Coords.to_string() + " " + Direction::to_string(dir));
            this->WalkTheLabyrinth(DestinationRoom->Linked[dir], DestinationRoom, Target);
        }

    }
    std::cout << "Returning" << std::endl;
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
