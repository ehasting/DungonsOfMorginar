//
// Created by eh on 09.01.2022.
//

#include "iohandler.hpp"
#include "core/keycodes.hpp"

namespace DofM
{
    void IOHandler::ProcessKeyPressEventCallback(KeyCodes::KeyPress detectedkeypress, std::vector<char> &keysequence)
    {
        std::string out;
        for (auto n: keysequence)
        {
            out += std::to_string((int) n);
        }
        this->WriteToBuffer(fmt::format("[QUEUE]: {}", out),
                            ScreenPos(24, 5), 16);
        switch (detectedkeypress)
        {
            case KeyCodes::KeyPress::UP:
                this->WriteToBuffer(fmt::format("[BUFFER]: UP"),
                                    ScreenPos(4, 5), 16);
                break;
            case KeyCodes::KeyPress::DOWN:
                this->WriteToBuffer(fmt::format("[BUFFER]: DOWN"),
                                    ScreenPos(4, 5), 16);
                break;
            case KeyCodes::KeyPress::ESC:
                this->WriteToBuffer(fmt::format("[BUFFER]: ESC"),
                                    ScreenPos(4, 5), 16);
                break;
            case KeyCodes::KeyPress::F5:
                this->WriteToBuffer(fmt::format("[BUFFER]: F5"),
                                    ScreenPos(4, 5), 16);
                break;
            case KeyCodes::KeyPress::BACKSPACE:
                if (!KeyLog.empty())
                    KeyLog.pop_back();
                break;
            case KeyCodes::KeyPress::DELETE:
                KeyLog.clear();
                break;
            case KeyCodes::KeyPress::ALPHA:
            case KeyCodes::KeyPress::NUMBER:
            case KeyCodes::KeyPress::SPECIAL_CHARACTER:
                KeyLog += keysequence[0];
                break;
            default:
                this->WriteToBuffer(fmt::format("[BUFFER]: {}", detectedkeypress),
                                    ScreenPos(4, 5), 16);
                break;
        }
    }

}