//
// Created by EgilHasting on 25/09/2022.
//

#include "linuxterminal.hpp"
#include <termios.h>

namespace DofM
{
    void LinuxTerminal::ScanKeyboardInput(std::shared_ptr<std::vector<char> > outdata)
    {
        while (true)
        {
            ReadCharBuffer[0] = EOF;
            ReadCharBuffer[1] = EOF;
            int l = read(STDIN_FILENO, this->ReadCharBuffer, 1);
            if (l > 0)
            {
                outdata->push_back(this->ReadCharBuffer[0]);
            }
            else
            {
                break;
            }
        }
    }
} // DofM