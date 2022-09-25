//
// Created by EgilHasting on 25/09/2022.
//

#ifndef DUNGONSOFMORGINAR_INPUTHANDLER_HPP
#define DUNGONSOFMORGINAR_INPUTHANDLER_HPP
#include "keycodes.hpp"

namespace DofM
{
    class InputHandler
    {
    public:
        virtual void ProcessKeyPressEventCallback(KeyCodes::KeyPress detectedkeypress, std::vector<char> &keysequence) = 0;
    };
}

#endif //DUNGONSOFMORGINAR_INPUTHANDLER_HPP
