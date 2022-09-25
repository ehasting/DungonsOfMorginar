//
// Created by EgilHasting on 25/09/2022.
//

#include "windowsterminal.hpp"
#include <memory>

namespace DofM
{
    void WindowsTerminal::ReadPlatformNativeTerminalSize(unsigned short &maxrow, unsigned short &maxcol)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(this->OutHandle, &csbi);
        maxcol = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        maxrow = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }

    void WindowsTerminal::ClearScreen()
    {
        const std::string clear("\033[2J\033[1;1H");
        std::cout << clear;
    }
    void WindowsTerminal::SetupNonBlockingTerminal()
    {
        // Set output mode to handle virtual terminal sequences
        this->OutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (this->OutHandle == INVALID_HANDLE_VALUE)
        {
            return;
        }
        this->InHandle = GetStdHandle(STD_INPUT_HANDLE);
        if (this->InHandle == INVALID_HANDLE_VALUE)
        {
            return;
        }

        if (!GetConsoleMode(this->OutHandle, &this->OriginalOutHandle))
        {
            return;
        }
        if (!GetConsoleMode(this->InHandle, &this->OriginalInHandle))
        {
            return;
        }

        DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
        DWORD dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT | ENABLE_WINDOW_INPUT ;

        DWORD dwOutMode = this->OriginalOutHandle | dwRequestedOutModes;
        if (!SetConsoleMode(this->OutHandle, dwOutMode))
        {
            // we failed to set both modes, try to step down mode gracefully.
            dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            dwOutMode = this->OriginalInHandle | dwRequestedOutModes;
            if (!SetConsoleMode(this->OutHandle, dwOutMode))
            {
                // Failed to set any VT mode, can't do anything here.
                return;
            }
        }

        DWORD dwInMode = this->OriginalInHandle | dwRequestedInModes;
        if (!SetConsoleMode(this->InHandle, dwInMode))
        {
            // Failed to set VT input mode, can't do anything here.
            return;
        }
        //BindCrtHandlesToStdHandles(true,true,true);
        std::cout.setf(std::ios::unitbuf);

        const std::string hidecursor("\033[?25l");
        std::cout << hidecursor;
        this->ClearScreen();
    }

    void WindowsTerminal::ScanKeyboardInput(std::shared_ptr<std::vector<char> > outdata)
    {
        //int timer = GetTickCount();
        //while(timer+10000 > GetTickCount())
        //{
        //}
        outdata->clear();
        long length;
        if (GetNumberOfConsoleInputEvents(this->InHandle, &this->ReadBufferBytesAvailable) != 0)
        {
            DWORD readchars, i;
            if (this->ReadBufferBytesAvailable > 0)
            {
                if (ReadConsoleInputA(this->InHandle, this->ReadCharBuffer, 16, &readchars) != 0)
                {
                    for (i = 0; i < readchars; i++)
                    {
                        outdata->push_back(this->ReadCharBuffer[i].Event.KeyEvent.uChar.AsciiChar);
                    }
                    //this->WriteToBuffer(fmt::format("ReadCode: {}", (int) readchars), ScreenPos(4, 7+readchars), 13);
                }
                //timer = GetTickCount();
            }
            //this->ScanKeyboardInput();
        }
    }
} // DofM