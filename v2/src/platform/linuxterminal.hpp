//
// Created by EgilHasting on 25/09/2022.
//

#ifndef DUNGONSOFMORGINAR_LINUXTERMINAL_HPP
#define DUNGONSOFMORGINAR_LINUXTERMINAL_HPP
#include "core/iterminal.hpp"
#include <iostream>
namespace DofM
{

    class LinuxTerminal : ITerminal
    {
        ~LinuxTerminal() {
            tcsetattr(0, TCSANOW, &this->OriginalTerminalSettings);
        }
        void ScanKeyboardInput(std::shared_ptr<std::vector<char> > outdata);
        void SetupNonBlockingTerminal()
        {
            struct termios newsettings;
            tcgetattr(0, &this->OriginalTerminalSettings);
            newsettings = this->OriginalTerminalSettings;
            newsettings.c_lflag &= ~ICANON;
            newsettings.c_lflag &= ~ECHO;
            newsettings.c_lflag &= ~ISIG;
            newsettings.c_cc[VMIN] = 0;
            newsettings.c_cc[VTIME] = 0;
            tcsetattr(0, TCSANOW, &newsettings);
            std::cout.setf(std::ios::unitbuf);
            this->ClearScreen();
            const std::string hidecursor("\033[?25l");
            std::cout << hidecursor;
        }

        void ReadPlatformNativeTerminalSize(unsigned short &maxrow, unsigned short &maxcol)
        {
            struct winsize w;
            ioctl(0, TIOCGWINSZ, &w);
            if (w.ws_row == 0 && w.ws_col == 0)
            {
                // we default if console reports bad dim (i.e. debug console etc)
                //this->RowMax = 42;
                //this->ColMax = 131;
                //ischanged = true;
                throw std::invalid_argument("This is not a compatible terminal, can't work with this.");
            }
            maxrow = w.ws_row;
            maxcol = w.ws_col;
        }
        void ClearScreen()
        {
            const std::string clear("\033[2J\033[1;1H");
            std::cout << clear;
        }
    private:
        char ReadCharBuffer[2];
        struct termios OriginalTerminalSettings;
    };

} // DofM

#endif //DUNGONSOFMORGINAR_LINUXTERMINAL_HPP
