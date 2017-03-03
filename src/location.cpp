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


Location::Location(std::string id, std::string description)
{
    this->ID = id;
    this->Description = description;
}

void Location::ShowRoom(AsciiRenderEngine &render, int lineoffset)
{
    int linecnt = lineoffset;
    render.Print("[" + this->ID + "]", AsciiRenderEngine::YELLOW, linecnt++);
    render.Print(this->Description, AsciiRenderEngine::BLUE, linecnt++);
    linecnt = linecnt + 3;
    if (this->ItemList.size() > 0)
    {
        render.Print("You also see", AsciiRenderEngine::WHITE, linecnt++);
        for (auto ex : this->ItemList)
            render.Print(" " + ex->Name + " " + ex->Placement +" (" + std::to_string(ex->ID) + ")", AsciiRenderEngine::GREEN, linecnt++);
    }
    render.Print("Exits:", AsciiRenderEngine::WHITE, linecnt++);
    for (auto ex : this->Linked)
    {
        std::string o = " - "  + ex.first + " (" + ex.second->ID  + ")";

        if (this->Blockers.count(ex.first) == 1)
        {
            render.Print(o + " *blocked (" + std::to_string(this->Blockers[ex.first]->ID) + ")", AsciiRenderEngine::RED, linecnt++);
        }
        else
        {
            render.Print(o, AsciiRenderEngine::GREEN, linecnt++);
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
