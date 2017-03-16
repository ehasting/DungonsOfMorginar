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
#include "location.h"
#include <string>
#include <iostream>

RoomCoords::RoomCoords(std::string SerializedRoomCoords)
{
    auto n = Tools::explode(SerializedRoomCoords, ',');
    this->X = std::stoi(n.at(0));
    this->Y = std::stoi(n.at(1));
    this->Z = std::stoi(n.at(2));
}

Location::Location(RoomCoords coord) : Location()
{
    this->Coords.X = coord.X;
    this->Coords.Y = coord.Y;
    this->Coords.Z = coord.Z;
}

Location::Location(std::string id, std::string description, RoomCoords coord) : Location(coord)
{
    this->ID = id;
    this->Description = description;
}

bool Location::IsNeigbour(RoomCoords roomCoords)
{
    int x = roomCoords.X;
    int y = roomCoords.Y;
    int z = roomCoords.Z;
    if ( ((this->Coords.X == x) || (this->Coords.X+1 == x) || (this->Coords.X-1 == x))
         && ((this->Coords.Y == y) || (this->Coords.Y+1 == x) || (this->Coords.Y-1 == x)) )
        return true;
    return false;
}

RoomCoords Location::GetNeightbourRoom(std::string direction)
{
    RoomCoords yourRoom = this->Coords;
    if (direction == "north")
    {
        yourRoom.Y++;
        return yourRoom;
    }
    if (direction == "south")
    {
        yourRoom.Y--;
        return yourRoom;
    }
    if (direction == "east")
    {
        yourRoom.X++;
        return yourRoom;
    }
    if (direction == "west")
    {
        yourRoom.X--;
        return yourRoom;
    }
    return RoomCoords();

}

std::string Location::IsVisitedLabel()
{
    if (this->IsVisited)
        return "Visisted";
    return "Not Visisted";
}

void Location::ShowRoom(AsciiRenderEngine &render, int lineoffset)
{
    int linecnt = lineoffset;
    render.Print(" - RoomName - [" + this->Coords.to_string() + "] - (" + this->IsVisitedLabel() +")" , RenderEngine::YELLOW, linecnt++);
    //render.Print(" - "+this->ID+" - [" + this->Coords.to_string() + "] - (" + this->IsVisitedLabel() +")" , RenderEngine::YELLOW, linecnt++);
    render.Print(this->Description, RenderEngine::BLUE, linecnt++);
    linecnt = linecnt + 3;
    if (this->ItemList.size() > 0)
    {
        render.Print("You also see", RenderEngine::WHITE, linecnt++);
        for (auto ex : this->ItemList)
            render.Print(" " + ex->Name + " " + ex->Placement +" (" + std::to_string(ex->ID) + ")", RenderEngine::GREEN, linecnt++);
    }
    render.Print("Exits:", RenderEngine::WHITE, linecnt++);
    for (auto ex : this->Linked)
    {
        std::string o = " - "  + ex.first + " (" + ex.second->ID  + ")";

        if (this->Blockers.count(ex.first) == 1)
        {
            render.Print(o + " *blocked (" + std::to_string(this->Blockers[ex.first]->ID) + ")", RenderEngine::RED, linecnt++);
        }
        else
        {
            render.Print(o, RenderEngine::GREEN, linecnt++);
        }
    }
}

bool Location::IsDirectionBlocked(std::string direction)
{
    for (auto block : this->Blockers)
    {
        if (this->Blockers.count(direction) == 1)
        {
            return true;
        }
    }
    return false;
}
