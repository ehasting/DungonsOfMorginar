//
// Created by eh on 09.01.2022.
//

#ifndef DUNGONSOFMORGINAR_IOHANDLER_HPP
#define DUNGONSOFMORGINAR_IOHANDLER_HPP
#include <string>
#include "core/nonblockingterminal.hpp"
namespace DofM
{
    class IOHandler : public NonBlockingTerminal
    {

    public:
        std::string KeyLog;
        IOHandler() : NonBlockingTerminal() { }
        ~IOHandler() {  }
        void ProcessKeyPressEventCallback(std::vector<char> &keysequence);
    };
}

#endif //DUNGONSOFMORGINAR_IOHANDLER_HPP
