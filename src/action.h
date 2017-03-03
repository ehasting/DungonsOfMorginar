#ifndef ACTION_H
#define ACTION_H
#include <string>
#include <functional>

class Action
{
public:
    Action(std::string keyword);
    std::string Keyword;
    std::string Id;
    std::function< void() > ActionFunction;
};

#endif // ACTION_H
