#ifndef ITEM_H
#define ITEM_H
#include <string>

class Item
{
public:
    Item(int id, std::string name, std::string placement, std::string desc)
    {
        this->ID = id;
        this->Name = name;
        this->Description = desc;
        this->Placement = placement;
    }
    bool State;
    std::string Description;
    std::string Name;
    std::string Placement;
    int ID;
};

#endif // ITEM_H
