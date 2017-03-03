#ifndef ROOM_H
#define ROOM_H
#include <vector>
#include <string>


class Room
{
public:


    Room();
    std::string Roomname;
    std::string Description;
    std::vector<std::pair<std::string, std::string> > exits;
    std::vector<std::pair<std::string,int> > item;
};

#endif // ROOM_H
