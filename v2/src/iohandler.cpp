//
// Created by eh on 09.01.2022.
//

#include "iohandler.hpp"
#include "core/keycodes.hpp"

namespace DofM
{
    IOHandler::ProcessOutcomes IOHandler::ProcessIfEscapeSequence(char &key)
    {
        if (!IsInKeyPressSequence)
        {
            if ((int) key == 27)
            {
                IsInKeyPressSequence = true;
                return IOHandler::ProcessOutcomes::ESC_START;
            }
        }
        else
        {
            if (!IsExpectingSequenceIdentifier && (int) key == 91)
            {
                IsExpectingSequenceIdentifier = true;
                return IOHandler::ProcessOutcomes::ESC_NEXT;
            }
        }
        if (IsInKeyPressSequence && IsExpectingSequenceIdentifier)
        {
            // Reset key sequence state
            IsExpectingSequenceIdentifier = false;
            IsInKeyPressSequence = false;
            return IOHandler::ProcessOutcomes::ESC_KEYCODE;
        }
        return IOHandler::ProcessOutcomes::REGULAR_KEY;
    }
    void IOHandler::ProcessKeyPressEventCallback(char &key)
    {
        switch (ProcessIfEscapeSequence(key))
        {
            case ESC_KEYCODE:
                switch (KeyCodes::EvaluateEscapedKey(key))
                {
                    case KeyCodes::EscapedKeys::UP:
                        this->WriteToBuffer(fmt::format("[BUFFER]: UP", key),
                                            ScreenPos(4, 5), 16);
                        break;
                    case KeyCodes::EscapedKeys::DOWN:
                        this->WriteToBuffer(fmt::format("[BUFFER]: DOWN", key),
                                            ScreenPos(4, 5), 16);
                        break;
                }
                break;
            case REGULAR_KEY:
                break;
        }

        if ((int) key != 10)
        {
            KeyLog.append( std::to_string((int) key) );
            KeyLog.append("|");
        }
        /*
                if (KeyCodes::IsKeyCode(inputbuffer, KeyCodes::CK_LEFT))
                {
                    // std::cout <<  " ! LEFT ! " << std::endl;
                } else if (KeyCodes::IsKeyCode(inputbuffer, KeyCodes::CK_UP))
                {
                    // std::cout <<  " ! UP ! " << std::endl;
                } else if (KeyCodes::IsKeyCode(inputbuffer, KeyCodes::CK_RIGHT))
                {
                    // std::cout <<  " ! RIGHT ! " << std::endl;
                } else if (KeyCodes::IsKeyCode(inputbuffer, KeyCodes::CK_DOWN))
                {
                    // std::cout <<  " ! DOWN ! " << std::endl;
                } else
                {
                    this->TextCommandBuffer << inputbuffer;
                }
                */
        //this->WriteToBuffer(fmt::format("[BUFFER]: {}", key),
        //                    ScreenPos(4, 5));
    }
}