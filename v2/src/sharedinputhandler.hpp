//
// Created by EgilHasting on 25/09/2022.
//

#ifndef DUNGONSOFMORGINAR_IOHANDLERSHARED_HPP
#define DUNGONSOFMORGINAR_IOHANDLERSHARED_HPP
#include "core/keycodes.hpp"
#include "core/screenpos.hpp"
#include "core/inputhandler.hpp"

namespace DofM
{
    class SharedInputHandler : public InputHandler
    {
    public:
        SharedInputHandler() : InputHandler()
        {}
        ~SharedInputHandler()
        {}
        void ProcessKeyPressEventCallback(KeyCodes::KeyPress detectedkeypress, std::vector<char> &keysequence)
        {
            //std::vector<TermRendringAttrs> emptyattr;
            std::string out;
            for (auto n: keysequence)
            {
                out += std::to_string((int) n);
            }
            //this->DebugCallback(fmt::format("[QUEUE]: {}", out),
            //                    ScreenPos(24, 5), 16, emptyattr);
            switch (detectedkeypress)
            {
                case KeyCodes::KeyPress::UP:
                    //this->DebugCallback(fmt::format("[BUFFER]: UP"),
                    //                    ScreenPos(4, 5), 16, emptyattr);
                    break;
                case KeyCodes::KeyPress::DOWN:
                    //this->DebugCallback(fmt::format("[BUFFER]: DOWN"),
                    //                    ScreenPos(4, 5), 16, emptyattr);
                    break;
                case KeyCodes::KeyPress::ESC:
                    //this->DebugCallback(fmt::format("[BUFFER]: ESC"),
                    //                   ScreenPos(4, 5), 16, emptyattr);
                    break;
                case KeyCodes::KeyPress::F5:
                    //this->DebugCallback(fmt::format("[BUFFER]: F5"),
                    //                  ScreenPos(4, 5), 16, emptyattr);
                    break;
                case KeyCodes::KeyPress::BACKSPACE:
                    if (!KeyLog.empty())
                        KeyLog.pop_back();
                    break;
                case KeyCodes::KeyPress::_DELETE:
                    KeyLog.clear();
                    break;
                case KeyCodes::KeyPress::ALPHA:
                case KeyCodes::KeyPress::NUMBER:
                case KeyCodes::KeyPress::SPECIAL_CHARACTER:
                    KeyLog += keysequence[0];
                    break;
                default:
                    //this->DebugCallback(fmt::format("[BUFFER]: {}", detectedkeypress),
                    //                   ScreenPos(4, 5), 16, emptyattr);
                    break;
            }
        }
    };
}

#endif //DUNGONSOFMORGINAR_IOHANDLERSHARED_HPP
