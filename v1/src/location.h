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


class Direction
{
public:
    enum Directions {
        NORTH,
        SOUTH,
        EAST,
        WEST,
        UP,
        DOWN,
        NA
    };
    static Directions from_string(std::string dir)
    {
        dir = Tools::getInstance().toLowercase(dir);
        if (dir == "north" || dir == "n")
            return Directions::NORTH;
        else if (dir == "south" || dir == "s")
            return Directions::SOUTH;
        else if (dir == "east" || dir == "e")
            return Directions::EAST;
        else if (dir == "west" || dir == "w")
            return Directions::WEST;
        else if (dir == "up" || dir == "u")
            return Directions::UP;
        else if (dir == "down" || dir == "d")
            return Directions::DOWN;
        return Directions::NA;
    }

    static std::string to_string(Directions dir)
    {
        switch (dir)
        {
        case Directions::NORTH:
            return "north";
            break;
        case Directions::SOUTH:
            return "south";
            break;
        case Directions::EAST:
            return "east";
            break;
        case Direction::WEST:
            return "west";
            break;
        case Directions::UP:
            return "up";
            break;
        case Directions::DOWN:
            return "down";
            break;
        default:
            return "na";
            break;
        }
    }

    static Directions GetBackWay(Directions going)
    {
        switch (going)
        {
        case Direction::Directions::NORTH:
            return Direction::Directions::SOUTH;

        case Direction::Directions::SOUTH:
            return Direction::Directions::NORTH;

        case Direction::Directions::EAST:
            return Direction::Directions::WEST;

        case Direction::Directions::WEST:
            return Direction::Directions::EAST;

        case Direction::Directions::UP:
            return Direction::Directions::DOWN;

        case Direction::Directions::DOWN:
            return Direction::Directions::UP;

        default:
            return going;
        }
    }
};


class RoomCoords
{
public:
    int X;
    int Y;
    int Z;

    RoomCoords()
    {
        if (!this->IsSet)
        {
            this->X = 1024;
            this->Y = 1024;
            this->Z = 1024;
            this->IsSet = true;
        }
    }

    RoomCoords(const RoomCoords &copy)
    {
        this->IsSet = true;
        this->X = copy.X;
        this->Y = copy.Y;
        this->Z = copy.Z;
    }

    RoomCoords(std::string SerializedRoomCoords);

    RoomCoords(int x, int y, int z)
    {
        this->IsSet = true;
        this->X = x;
        this->Y = y;
        this->Z = z;
    }

    bool operator ==(const RoomCoords &Right) const
    {
        if ( (this->X == Right.X) &&
             (this->Y == Right.Y) &&
             (this->Z == Right.Z) )
        {
            return true;
        }
        return false;
    }
    std::string to_string()
    {
        return std::to_string(this->X) + "," + std::to_string(this->Y) + "," + std::to_string(this->Z);
    }

    bool IsNeigbour(RoomCoords &roomCoords)
    {
        int x = roomCoords.X;
        int y = roomCoords.Y;
        int z = roomCoords.Z;
        if ( ((this->X == x) || (this->X+1 == x) || (this->X-1 == x))
             && ((this->Y == y) || (this->Y+1 == x) || (this->Y-1 == x)) )
            return true;
        return false;
    }

    void Move(Direction::Directions &direction)
    {
        this->MoveReference(direction, *this);
    }

    RoomCoords GetNeightbourRoomCoords(Direction::Directions &direction)
    {
        RoomCoords yourRoom(this->X, this->Y, this->Z);
        this->MoveReference(direction, yourRoom);
        return yourRoom;
    }
private:
    bool IsSet;
    void MoveReference(Direction::Directions &direction, RoomCoords &coords)
    {
        switch (direction)
        {
        case Direction::Directions::NORTH:
            coords.Y++;
            break;
        case Direction::Directions::SOUTH:
            coords.Y--;
            break;
        case Direction::Directions::EAST:
            coords.X++;
            break;
        case Direction::Directions::WEST:
            coords.X--;
            break;
        default:
            std::cout << "Direction Unknown (" << direction << ")" << std::endl;
            break;
        }
    }

};





class Location
{
public:
    Location() {
        this->IsVisited = false;
        std::cout << "location construction " << this->Coords.to_string();
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
    std::map<Direction::Directions, Location*> Linked;
    std::map<Direction::Directions, Item*> Blockers;
    std::vector<Item*> ItemList;
    RoomCoords Coords;
    void ShowRoom(AsciiRenderEngine &render, int &lineoffset);
    void DrawRoom(AsciiRenderEngine &render, int &lineoffset);
    bool IsDirectionBlocked(Direction::Directions diretion);
    bool IsVisited;

};

#endif // LOCATION_H
