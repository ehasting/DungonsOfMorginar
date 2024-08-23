//
// Created by eh on 30.03.23.de
//

#include <fmt/format.h>
#include <string>

#include "actionparser.hpp"


namespace DofM
{
    std::string ActionParser::Parse(std::string &sentence)
    {

        return fmt::format("Parsing: {}", sentence.c_str());
    }
}