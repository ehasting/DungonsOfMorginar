//
// Created by eh on 1/8/22.
//

#include "nonblockingterminal.hpp"
#include <sys/ioctl.h>
#include <stdio.h>
#include <algorithm>

namespace DofM
{
    NonBlockingTerminal::NonBlockingTerminal()
    {
        this->ReadTerminalSize();
    }

    NonBlockingTerminal::~NonBlockingTerminal()
    {
        if (this->IsInNonBlockingMode)
        {
            tcsetattr(0, TCSANOW, &this->OriginalTerminalSettings);
            std::cout << this->GotoXY(ScreenPos(this->ColMax, this->RowMax)) << std::endl;
            const std::string showcursor("\033[?25h");
            std::cout << showcursor;
        }
        //std::cout << std::endl;
        //std::cout << "content of ScreenBuffer" << std::endl;
        //for (auto c : this->ScreenBuffer)
        //{
        //    std::cout << c;
        //}
        //std::cout << std::endl;
    }

    void NonBlockingTerminal::WriteToBuffer(std::string text, ScreenPos pos)
    {
        std::vector<TermRendringAttrs> emptyattr;
        this->WriteToBuffer(text, pos, emptyattr);
    }
    void NonBlockingTerminal::WriteToBuffer(std::string text, ScreenPos pos, std::vector<TermRendringAttrs> &attrs)
    {
        this->DrawMutex.lock();
        bool hasrendring = false;
        std::string renderingbuffer;

        // Rendring codes
        if (attrs.size() > 0)
        {
            hasrendring = true;
            std::string sep = "";
            renderingbuffer.append("\033[");
            for (auto n: attrs)
            {
                renderingbuffer.append(sep);
                renderingbuffer.append(std::to_string((int)n));
                sep = ";";
            }
            renderingbuffer.append("m");
            auto startkey = this->GetRenderingTableLookupKey(pos);
            this->RenderingTable[startkey] = renderingbuffer;
        }
        this->CheckIfOnScreen(pos, text.length());

        unsigned int bufferstartindex = this->GetBufferPosition(pos);


        if (hasrendring)
        {
            unsigned int stopcol = bufferstartindex + text.length();
            auto endkey = this->GetRenderingTableLookupKey(ScreenPos(stopcol, pos.Row()));
            this->RenderingTable[endkey] = "\033[0m";
        }

        for (auto c : text)
        {
            this->ScreenBuffer[bufferstartindex] = c;
            bufferstartindex++;
        }
        this->DrawMutex.unlock();
    }


    std::string NonBlockingTerminal::GotoXY(ScreenPos pos)
    {
        this->CheckIfOnScreen(pos, 0);
        return fmt::format("\033[{};{}H", pos.Row()+1, pos.Col()+1 );
    }
    void NonBlockingTerminal::ReadTerminalSize()
    {
        // endline steals a hidden character which wraps in terminal, rendring without gains an additional char.
        unsigned int endlineadjust = 0;
        // Find terminal window size.
        bool ischanged = false;
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);

        if (w.ws_row == 0 && w.ws_col == 0)
        {
            // we default if console reports bad dim (i.e. debug console etc)
            this->RowMax = 42;
            this->ColMax = 131;
            ischanged = true;
        }
        else
        {
            // update if changed
            if (this->RowMax != w.ws_row-endlineadjust)
            {
                this->RowMax = w.ws_row-endlineadjust;
                ischanged = true;
            }
            // update if changed
            if (this->ColMax != w.ws_col-endlineadjust)
            {
                this->ColMax = w.ws_col-endlineadjust;
                ischanged = true;
            }
        }
        // any change will triggger a full redraw - flush buffer and redraw
        if (ischanged)
        {

            this->ResizeScreenBuffer();
            this->WriteToBuffer(fmt::format("changed term to : {}x{}", this->RowMax, this->ColMax), ScreenPos(4,10));
            this->Redraw();
        }

    }
    void NonBlockingTerminal::SetupNonBlockingTerminal()
    {
        IsInNonBlockingMode = true;
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
        this->ReadTerminalSize();
        const std::string hidecursor("\033[?25l");
        std::cout << hidecursor;
    }

    void NonBlockingTerminal::Redraw()
    {
        if ((RedrawCounter % 10) == 0)
        {
            this->ReadTerminalSize();
        }
        this->DrawMutex.lock();
        std::string outbuffer;
        //std::cout << this->GotoXY(ScreenPos(0,0));
        unsigned int row = 1;
        unsigned int linecursor = 0;
        unsigned int buffercursor = 0;
        for (char &c : this->ScreenBuffer)
        {
            auto formating = GetRendringTableData(buffercursor);
            if (formating != std::string())
                outbuffer.append(formating);
            outbuffer += c;
            linecursor++;
            if (linecursor >= this->ColMax)
            {
                auto previousline = GetLiveScreenBufferLine(buffercursor);
                if (previousline != outbuffer)
                {
                    std::cout << this->GotoXY(ScreenPos(1, row))  << outbuffer;

                }
                UpdateLiveScreenBufferLine(buffercursor, outbuffer);
                outbuffer.clear();
                linecursor = 0;
                row++;
            }
            buffercursor++;
             //outbuffer += '\n';
        }
        this->DrawMutex.unlock();
        RedrawCounter++;
    }
    void NonBlockingTerminal::ClearScreen()
    {
        const std::string clear("\033[2J\033[1;1H");
        std::cout << clear;
    }
    void NonBlockingTerminal::ScanKeyboardInput()
    {
        ReadCharBuffer[0] = EOF;
        ReadCharBuffer[1] = EOF;
        int l = read(STDIN_FILENO, this->ReadCharBuffer, 1);
        if (l > 0)
        {
            this->IOMutex.lock();
            InputBuffer[InputBufferCursor] = this->ReadCharBuffer[0];
            this->WriteToBuffer(fmt::format("ReadCode: {}", std::to_string((int) this->ReadCharBuffer[0])), ScreenPos(4, 7 ));
            InputBufferCursor++;
            this->IOMutex.unlock();
        }
    }

    std::string_view NonBlockingTerminal::GetAndClearNonBlockingIOBuffer()
    {
        if (this->InputBufferCursor > 0)
        {
            this->IOMutex.lock();
            std::string_view currentbuffer(this->InputBuffer, this->InputBufferCursor);
            this->InputBufferCursor = 0;
            this->IOMutex.unlock();
            return currentbuffer;
        }
        return std::string_view();
    }
}