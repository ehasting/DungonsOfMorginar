#ifndef LOCATION_H
#define LOCATION_H
#include <string>
#include <vector>
#include <map>
#include "item.h"
#include "location.h"
#include "asciirenderengine.h"

class Location
{
public:
    Location(std::string id, std::string description);
    bool operator ==(const Location &Right) const
    {
        return (this->ID == Right.ID);
    }
    std::string ID;
    std::string Description;
    std::map<std::string, Location*> Linked;
    std::map<std::string, Item*> Blockers;
    std::vector<Item*> ItemList;

    void ShowRoom(AsciiRenderEngine &render, int lineoffset);
    bool IsDirectionBlocked(std::string diretion);

};

#endif // LOCATION_H
