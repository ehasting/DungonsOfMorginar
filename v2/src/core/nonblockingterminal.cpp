//
// Created by eh on 1/8/22.
//

#include "nonblockingterminal.hpp"
#include <stdio.h>
#include <algorithm>
#include <thread>
#include "utf8.h"
namespace DofM
{
    NonBlockingTerminal::NonBlockingTerminal(std::shared_ptr<ITerminal> terminaltype)
        : Terminal(terminaltype)
    {
        this->Terminal->SetupNonBlockingTerminal();
        this->ReadTerminalSize();
        this->IsReady = true;
    }

    NonBlockingTerminal::~NonBlockingTerminal()
    {
    }


    void NonBlockingTerminal::WriteToBuffer(std::string text, ScreenPos pos, SDL_Color fg, int priority)
    {
        // TODO:  Implement "z-index" of priorty to force text on top (not allow overwrite over higher prioirty)
        if (!this->IsReady)
        {
            return;
        }

        unsigned int bufferstartindex = this->GetBufferPosition(pos);
        unsigned int charcount = 0;
        // should be one for loop
        try
        {
            char* str = (char*)text.c_str();
            char* str_i = str;                  // string iterator
            char* end = str+strlen(str)+1;      // end iterator
            do
            {
                uint32_t code = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
                if (code == 0)
                    continue;
                charcount++;
                char symbol[5] = {0};
                utf8::append(code, symbol); // copy code to symbol
                std::string newchar(symbol);
                this->ScreenBuffer[bufferstartindex].SetScreenCharacter(newchar, fg, priority);
                /*
                if (this->ScreenBuffer[bufferstartindex].IsPrioritized(priority))
                {

                    //this->ScreenBuffer[bufferstartindex] = ScreenCharacter(newchar, fg, priority);
                }
                else
                {
                    fmt::print("Ignoring: {}\n", pos.ToString());
                }
                this->ScreenBuffer[bufferstartindex].FlagAsUsed();
                */
                bufferstartindex++;
            } while ( str_i < end );
        }
        catch(const std::exception& e)
        {
            fmt::print("Error: {}", e.what());
        }
        this->CheckIfOnScreen(pos, charcount);
    }

    void NonBlockingTerminal::ReadTerminalSize()
    {
        unsigned short maxrow, maxcol;

        this->Terminal->ReadPlatformNativeTerminalSize(maxrow, maxcol);

        // endline steals a hidden character which wraps in terminal, rendring without gains an additional char.
        unsigned int endlineadjust = 0;

        // Find terminal window size.
        bool ischanged = false;

        // update if changed
        if (this->RowMax != maxrow - endlineadjust)
        {
            this->RowMax = maxrow - endlineadjust;
            ischanged = true;
        }

        // update if changed
        if (this->ColMax != maxcol - endlineadjust)
        {
            this->ColMax = maxcol - endlineadjust;
            ischanged = true;
        }

        // any change will triggger a full redraw - flush buffer and redraw
        if (ischanged)
        {
            this->ResizeScreenBuffer();
            this->WriteToBuffer(fmt::format("changed term to : {}x{}", this->RowMax, this->ColMax), ScreenPos(5,1));
            this->Terminal->ClearScreen();
            //this->Redraw();
        }
    }

    void NonBlockingTerminal::FlipDrawbuffer()
    {
        // Copy to drawbuffer - we should move this.
        this->DrawScreenBuffer = this->ScreenBuffer;
        this->ClearScreenBuffer();
    }

    void NonBlockingTerminal::Redraw()
    {
        this->FlipDrawbuffer();
        if (!this->IsReady)
            return;
        if ((RedrawCounter % 10) == 0)
        {
            this->ReadTerminalSize();
        }

        this->Terminal->ClearScreen();

        unsigned int row = 0;
        unsigned int linecursor = -1; // because we increase before print - we need to start on -1


        for (auto &c : this->DrawScreenBuffer)
        {
            linecursor++;
            if (linecursor >= this->ColMax)
            {
                linecursor = 0;
                row++;
            }
            this->Terminal->PrintLetter(ScreenPos(linecursor, row), c);
        }
        this->Terminal->UpdateScreen();
        RedrawCounter++;
    }

}
