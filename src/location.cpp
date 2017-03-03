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
