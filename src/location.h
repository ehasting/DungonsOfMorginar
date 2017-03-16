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
#ifndef LOCATION_H
#define LOCATION_H
#include <string>
#include <vector>
#include <map>
#include "item.h"
#include "location.h"
#include "asciirenderengine.h"
#include "tools.h"

class RoomCoords
{
public:
    RoomCoords() {}


    RoomCoords(const RoomCoords &copy)
    {
        this->X = copy.X;
        this->Y = copy.Y;
        this->Z = copy.Z;
    }

    RoomCoords(std::string SerializedRoomCoords);

    RoomCoords(int x, int y, int z) : RoomCoords()
    {
        this->X = x;
        this->Y = y;
        this->Z = z;
    }
    std::string to_string()
    {
        return std::to_string(this->X) + "," + std::to_string(this->Y) + "," + std::to_string(this->Z);
    }

    int X;
    int Y;
    int Z;
};

class Location
{
public:

    Location() {
        this->IsVisited = false;
    }
    Location(RoomCoords coord);
    Location(std::string id, std::string description, RoomCoords coord);
    bool operator ==(const Location &Right) const
    {
        return (this->ID == Right.ID);
    }
    std::string IsVisitedLabel();
    std::string ID;
    std::string Description;
    std::map<std::string, Location*> Linked;
    std::map<std::string, Item*> Blockers;
    std::vector<Item*> ItemList;
    RoomCoords Coords;
    bool IsNeigbour(RoomCoords roomCoords);
    RoomCoords GetNeightbourRoom(std::string direction);
    void ShowRoom(AsciiRenderEngine &render, int lineoffset);
    bool IsDirectionBlocked(std::string diretion);
    bool IsVisited;

};

#endif // LOCATION_H
