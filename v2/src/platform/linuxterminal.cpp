//
// Created by EgilHasting on 25/09/2022.
//

#include "linuxterminal.hpp"
#include <termios.h>

namespace DofM
{
    void LinuxTerminal::ScanKeyboardInput(std::shared_ptr<std::vector<char> > outdata)
    {
        int l = EOF;
        ReadCharBuffer[0] = EOF;
        ReadCharBuffer[1] = EOF;
        while (l = read(STDIN_FILENO, this->ReadCharBuffer, 1) > 0)
        {
            outdata->push_back(this->ReadCharBuffer[0]);
            ReadCharBuffer[0] = EOF;
            ReadCharBuffer[1] = EOF;
        }
     
    }
} // DofM