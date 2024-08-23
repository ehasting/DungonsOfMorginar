//
// Created by eh on 30.03.23.
//

#include <chrono>
#include <typeinfo>
#include <string>
#include <memory>

#ifndef DUNGONSOFMORGINAR_ACTIONPARSER_HPP
#define DUNGONSOFMORGINAR_ACTIONPARSER_HPP

namespace DofM
{
    class ActionParser
    {
    public:
        std::string Parse(std::string &sentence);
    };
}

#endif //DUNGONSOFMORGINAR_ACTIONPARSER_HPP
