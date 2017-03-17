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
#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H
#include <vector>
#include "location.h"
#include <random>

class MazeGenerator
{
public:
    MazeGenerator();
    std::map<std::string, Location> TheMaze;
    void GenerateNeigbour(Location &source, Direction::Directions goingto, Location &neighbour);
private:
    int RoomIdIndex;
    std::vector<int> RoomIDList;

    Direction::Directions GetNextWay(Direction::Directions commingfrom);
    std::string GenerateRoomID(int index, Direction::Directions fromdirection, bool nextroom)
    {
        if (nextroom)
            this->RoomIdIndex++;
        std::string returnid = std::to_string(index) + "-" + std::to_string(this->RoomIdIndex) + "_" +Direction::to_string(fromdirection);
        return returnid;
    }


};

#endif // MAZEGENERATOR_H
