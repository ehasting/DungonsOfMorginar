//
// Created by eh on 1/8/22.
//

#include "nonblockingterminal.hpp"
#include <sys/ioctl.h>
#include <stdio.h>
#include <algorithm>
#include <thread>
namespace DofM
{
    NonBlockingTerminal::NonBlockingTerminal()
    {
        this->ReadTerminalSize();
        this->KeyboardEventThread = std::make_shared<std::thread>([this]{ this->CheckForKeyboardEventWorker(); });
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
        this->KeyboardEventThread->join();
        //std::cout << std::endl;
        //std::cout << "content of ScreenBuffer" << std::endl;
        //for (auto c : this->ScreenBuffer)
        //{
        //    std::cout << c;
        //}
        //std::cout << std::endl;
    }

    void NonBlockingTerminal::WriteToBuffer(std::string text, ScreenPos pos, unsigned int maxtextlength)
    {
        std::vector<TermRendringAttrs> emptyattr;
        this->WriteToBuffer(text, pos, maxtextlength, emptyattr);
    }
    void NonBlockingTerminal::WriteToBuffer(std::string text, ScreenPos pos, unsigned int maxtextlength, std::vector<TermRendringAttrs> &attrs)
    {
        if (!this->IsReady)
            return;
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
        unsigned int cleanbufferstartindex = bufferstartindex;

        if (hasrendring)
        {
            unsigned int stopcol = bufferstartindex + text.length();
            auto endkey = this->GetRenderingTableLookupKey(ScreenPos(stopcol, pos.Row()));
            this->RenderingTable[endkey] = "\033[0m";
        }

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


    std::string NonBlockingTerminal::GotoXY(ScreenPos pos)
    {
        this->CheckIfOnScreen(pos, 0);
        return fmt::format("\033[{};{}H", pos.Row(), pos.Col());
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
            //this->RowMax = 42;
            //this->ColMax = 131;
            //ischanged = true;
            throw std::invalid_argument("This is not a compatible terminal, can't work with this.");
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
            this->WriteToBuffer(fmt::format("changed term to : {}x{}", this->RowMax, this->ColMax), ScreenPos(5,1), 25);
            this->ClearScreen();
            //this->Redraw();
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
        IsReady = true;
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
        std::string outbuffer;

        //std::cout << this->GotoXY(ScreenPos(1,1));
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
                // Only draw if line has changed since last drawing
                // Partly "redraw"
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
                InputQueue.push(this->ReadCharBuffer[0]);
                this->IOMutex.unlock();
                this->WriteToBuffer(fmt::format("ReadCode: {}", std::to_string((int) this->ReadCharBuffer[0])),
                                    ScreenPos(4, 7),13);
                this->ScanKeyboardInput();
            }
            else
                // if we found data, do another read to check if buffer still has content
                this->ProcessKeyPressEventQueue();

    }

    void NonBlockingTerminal::ProcessKeyPressEventQueue()
    {
        this->IOMutex.lock();
        bool inescsequence = false;
        bool csifound = false;
        int csiindex = 0;
        std::vector<char> sequence;
        // Process whatever is in    the queue untill empty or we return in loop.
        while (!this->InputQueue.empty() && this->IsRunning)
        {
            this->WriteToBuffer(fmt::format("InputQueue Size: {} ({})", this->InputQueue.size(), (int)this->InputQueue.front()), ScreenPos(15,15), 28);
            auto n = this->InputQueue.front();

            // ONLY enter here if we are in a escape seqeunce, before CSI is found.
            if ((n == KeyCodes::KeySequence::ASCII::ESC || inescsequence) && !csifound)
            {
                if (n == KeyCodes::KeySequence::ASCII::ESC && inescsequence)
                {
                    // Previous escape was probably escape.. lets process that.
                    this->ProcessKeyPressEventCallback(KeyCodes::KeyPress::ESC, sequence);
                    // Do not pop, since we are now processing previous sequence.
                    break;  // with return we will continue with reset state, but we could also clean state then continue.
                }
                else if (n == KeyCodes::KeySequence::ASCII::CSI && inescsequence)
                {
                    csifound = true;
                    csiindex = 0;
                }
                // escape sequence is progressing correctly.
                inescsequence = true;
                sequence.push_back(n);
                this->InputQueue.pop();
            }
            else if (csifound)
            {
                sequence.push_back(n);
                this->InputQueue.pop();
                csiindex++;
                // this is an escape sequence that expects ONE letter. i.e. <esc>[A
                if (KeyCodes::IsLetter({n}) && csiindex == 1)
                {
                    auto foundkeypress = KeyCodes::MatchSequence(sequence);
                    this->ProcessKeyPressEventCallback(foundkeypress, sequence);
                    break;
                }
                else if (csiindex == 4)
                {
                    // we will throw away the sequence if its beyond 4 letters
                    break;
                }
                else
                {
                    auto foundkeypress = KeyCodes::MatchSequence(sequence);
                    if (foundkeypress != KeyCodes::KeyPress::UNDETECTED_ESCAPE_SEQUENCE)
                    {
                        this->ProcessKeyPressEventCallback(foundkeypress, sequence);
                        break;
                    }
                }
            }
            else
            {
                sequence.push_back(n);
                this->InputQueue.pop();
                // we will try to validate this to be a typable key
                auto foundkeypress = KeyCodes::MatchKey(n);

                if (foundkeypress == KeyCodes::KeyPress::NO_KEY )
                {
                    // Ignore whatever.. we cant match on anything.. just wipe and break the loop
                    sequence.clear();
                    break;
                }
                this->ProcessKeyPressEventCallback(foundkeypress, sequence);
                sequence.clear();
            }


        }

        this->IOMutex.unlock();
    }
}