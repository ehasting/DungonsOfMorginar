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
        enum ProcessOutcomes
        {
            ESC_START,
            ESC_NEXT,
            ESC_KEYCODE,
            REGULAR_KEY
        };
        bool IsInKeyPressSequence = false;
        bool IsExpectingSequenceIdentifier = false;
        ProcessOutcomes ProcessIfEscapeSequence(char &key);
    public:
        std::string KeyLog;

        IOHandler() : NonBlockingTerminal()
        {

        }

        ~IOHandler()
        {

        }

        void ProcessKeyPressEventCallback(char &key);
    };
}

#endif //DUNGONSOFMORGINAR_IOHANDLER_HPP
