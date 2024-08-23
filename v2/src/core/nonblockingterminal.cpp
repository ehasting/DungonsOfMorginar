//
// Created by eh on 1/8/22.
//

#include "nonblockingterminal.hpp"
#include <stdio.h>
#include <algorithm>
#include <thread>
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

    void NonBlockingTerminal::WriteToBuffer(std::string text, ScreenPos pos, unsigned int maxtextlength)
    {
        std::vector<TermRendringAttrs> emptyattr;
        this->WriteToBuffer(text, pos, maxtextlength, {1,1,1,1});
    }
    void NonBlockingTerminal::WriteToBuffer(std::string text, ScreenPos pos, unsigned int maxtextlength, SDL_Color fg)
    {
        if (!this->IsReady)
            return;
        this->DrawMutex.lock();

        auto startkey = this->GetRenderingTableLookupKey(pos);
        this->RenderingTable[startkey] = fg;

        this->CheckIfOnScreen(pos, text.length());

        unsigned int bufferstartindex = this->GetBufferPosition(pos);
        unsigned int cleanbufferstartindex = bufferstartindex;

        // should be one for loop
        for (unsigned int x = cleanbufferstartindex; x < cleanbufferstartindex+maxtextlength; x++)
        {
            this->ScreenBuffer[x] = ' ';
        }
        for (auto c : text)
        {
            this->ScreenBuffer[bufferstartindex] = c;
            bufferstartindex++;
        }
        this->DrawMutex.unlock();
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
        if (this->RowMax != maxrow-endlineadjust)
        {
            this->RowMax = maxrow-endlineadjust;
            ischanged = true;
        }
        // update if changed
        if (this->ColMax != maxcol-endlineadjust)
        {
            this->ColMax = maxcol-endlineadjust;
            ischanged = true;
        }

        // any change will triggger a full redraw - flush buffer and redraw
        if (ischanged)
        {
            this->ResizeScreenBuffer();
            this->WriteToBuffer(fmt::format("changed term to : {}x{}", this->RowMax, this->ColMax), ScreenPos(5,1), 25);
            this->Terminal->ClearScreen();
            //this->Redraw();
        }
    }

    void NonBlockingTerminal::Redraw()
    {
        if (!this->IsReady)
            return;
        if ((RedrawCounter % 10) == 0)
        {
            this->ReadTerminalSize();
        }
        this->DrawMutex.lock();
        this->Terminal->ClearScreen();
        std::string outbuffer;

        unsigned int row = 1;
        unsigned int linecursor = 0;
        unsigned int buffercursor = 0;
        SDL_Color formating;
        for (char &c : this->ScreenBuffer)
        {
            formating = GetRendringTableData(buffercursor);
            outbuffer += c;
            linecursor++;
            if (linecursor >= this->ColMax)
            {
                this->Terminal->PrintTextCopy(1,row, outbuffer, formating);
                outbuffer.clear();
                linecursor = 0;
                row++;
            }

            buffercursor++;
             //outbuffer += '\n';
        }
        this->Terminal->UpdateScreen();
        this->DrawMutex.unlock();
        RedrawCounter++;
    }

}