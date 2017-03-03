#ifndef LOCATIONS_H
#define LOCATIONS_H
#include <vector>
#include "room.h"
#include "location.h"
#include "asciirenderengine.h"

class Locations
{
public:
    Locations();
    void FindNewRoom(std::string);
    std::vector<Location> Map;

    Location *CurrentRoom;

    bool CommandIsKnown;

    void Init()
    {
        this->CurrentRoom = &this->Map.at(0);
    }


};

#endif // LOCATIONS_H
