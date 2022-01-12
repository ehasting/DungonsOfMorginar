//
// Created by eh on 1/8/22.
//

#ifndef DUNGONSOFMORGINAR_NONBLOCKINGTERMINAL_HPP
#define DUNGONSOFMORGINAR_NONBLOCKINGTERMINAL_HPP

#include <string>
#include <locale>
#include <vector>
#include <random>
#include <iostream>
#include <mutex>
#include <chrono>
#include <typeinfo>
#include <memory>
#include <map>
#include "fmt/core.h"
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <queue>
#include <thread>
#include "screenpos.hpp"
#include "keycodes.hpp"


/*
 * Usage:
 * Before usage:
 *      SetupNonBlockingTerminal();
 * NonBlockingTerminal term;
 * in a non-blocking loop:
 *      ScanKeyboardInput();
 * in same or other non-blocking loop:
 * auto n = GetAndClearNonBlockingIOBuffer(); if (n != NonBlockingTerminal::EmptyBuffer()) { .. action }
 *
 */
namespace DofM
{
    class NonBlockingTerminal
    {
    public:
        void Terminate()
        {
            this->IsRunning = false;
        }
        enum TermRendringAttrs
        {
            RESET = 0,
            BOLD = 1,
            BLINK = 5,
            REV_BLINK = 7,

            BG_BLACK = 40,
            BG_RED = 41,
            BG_GREEN = 42,
            BG_BROWN = 43,
            BG_BLUE = 44,
            BG_MAGENTA = 45,
            BG_CYAN = 46,
            BG_WHITE = 47,
            BG_BLACK_BR = 100,
            BG_RED_BR = 101,
            BG_GREEN_BR = 102,
            BG_BROWN_BR = 103,
            BG_BLUE_BR = 104,
            BG_MAGENTA_BR = 105,
            BG_CYAN_BR = 106,
            BG_WHITE_BR = 107,
            FG_BLACK = 30,
            FG_RED = 31,
            FG_GREEN = 32,
            FG_BROWN = 33,
            FG_BLUE = 34,
            FG_MAGENTA = 35,
            FG_CYAN = 36,
            FG_WHITE = 37,
            FG_BLACK_BR = 90,
            FG_RED_BR = 91,
            FG_GREEN_BR = 92,
            FG_BROWN_BR = 93,
            FG_BLUE_BR = 94,
            FG_MAGENTA_BR = 95,
            FG_CYAN_BR = 96,
            FG_WHITE_BR = 97
        };
    private:
        bool IsReady = false;
        bool IsRunning = true;
        bool IsInNonBlockingMode = false;
        std::queue<char> InputQueue;
        struct termios OriginalTerminalSettings;
        std::mutex IOMutex;
        std::mutex DrawMutex;
        char ReadCharBuffer[2];
        void ProcessKeyPressEventQueue();
        std::string GetRenderingTableLookupKey(ScreenPos pos)
        {
            return std::to_string(pos.Col()) + "_" + std::to_string(pos.Row());
        }
        unsigned int ScreenBufferLength;
        std::vector<char> ScreenBuffer;
        void ResizeScreenBuffer()
        {
            this->DrawMutex.lock();
            this->LiveScreenBuffer.clear();
            this->ScreenBufferLength = this->RowMax * this->ColMax;
            this->ScreenBuffer.clear();
            this->ScreenBuffer.resize(this->ScreenBufferLength);
            std::fill(this->ScreenBuffer.begin(), this->ScreenBuffer.end(), ' ');
            this->DrawMutex.unlock();
        }


        std::map<std::string, std::string> LiveScreenBuffer;
        std::string GetLiveScreenBufferLine(unsigned int bufferpos)
        {
            return this->GetLiveScreenBufferLine(this->GetScreenPosition(bufferpos));
        }
        std::string GetLiveScreenBufferLine(ScreenPos pos)
        {
            auto key = this->GetRenderingTableLookupKey(pos);
            auto f = this->LiveScreenBuffer.find(key);
            if (f != this->LiveScreenBuffer.end())
            {
                return this->LiveScreenBuffer[key];
            }
            return std::string();
        }
        void UpdateLiveScreenBufferLine(unsigned int bufferpos, std::string line)
        {
            this->UpdateLiveScreenBufferLine(this->GetScreenPosition(bufferpos), line);
        }
        void UpdateLiveScreenBufferLine(ScreenPos pos, std::string line)
        {
            auto key = this->GetRenderingTableLookupKey(pos);
            this->LiveScreenBuffer[key] = line;
        }
        std::map<std::string, std::string> RenderingTable;
        std::string GetRendringTableData(ScreenPos pos)
        {
            auto key = GetRenderingTableLookupKey(pos);
            auto f = this->RenderingTable.find(key);
            if (f != this->RenderingTable.end())
            {
                return this->RenderingTable[key];
            }
            return std::string();
        }
        std::string GetRendringTableData(unsigned int bufferpos)
        {
            return this->GetRendringTableData(this->GetScreenPosition(bufferpos));
        }
        void ReadTerminalSize();
        unsigned int GetBufferPosition(ScreenPos pos)
        {
            unsigned int rval = ((pos.Row() * this->ColMax) + pos.Col()) - 1;
            if (rval > this->ScreenBufferLength)
            {
                throw std::invalid_argument(fmt::format("GetBufferPosition out of bound: {}", rval));
            }
            return rval;
        }
        ScreenPos GetScreenPosition(unsigned int bufferposition)
        {
            unsigned int row = std::floor<unsigned int>(bufferposition / this->ColMax);
            unsigned int col = bufferposition - (row * this->ColMax);

            ScreenPos pos;
            pos.SetRow(row+1);
            pos.SetCol(col+1);
            return pos;
        }
        std::string GotoXY(ScreenPos pos);
        unsigned int RedrawCounter = 0;
        std::shared_ptr<std::thread> KeyboardEventThread;
    public:
        NonBlockingTerminal();
        ~NonBlockingTerminal();


        unsigned short int RowMax = 0;
        unsigned short int ColMax = 0;

        void SetupNonBlockingTerminal();

        virtual void ProcessKeyPressEventCallback(KeyCodes::KeyPress detectedkeypress, std::vector<char> &keysequence) = 0;

        void ScanKeyboardInput();

        void Redraw();

        void WriteToBuffer(std::string text, ScreenPos pos, unsigned int maxtextlength, std::vector<TermRendringAttrs> &attrs );
        void WriteToBuffer(std::string text, ScreenPos pos, unsigned int maxtextlength);
        void CheckIfOnScreen(ScreenPos pos, unsigned int textlength)
        {
            if (pos.Col() > this->ColMax)
            {
                std::string error = fmt::format("col out of bounds (max: {}, got: {}", this->ColMax, pos.Col());
                std::cout << error << std::endl;
                throw std::invalid_argument(error);
            }
            if (pos.Row() > this->RowMax)
            {
                std::string error = fmt::format("row out of bounds (max: {}, got: {}", this->RowMax, pos.Row());
                std::cout << error << std::endl;
                throw std::invalid_argument(error);
            }
            if (pos.Col()+textlength > this->ColMax)
            {
                std::string error = fmt::format("textlength out of bounds (max: {}, got: {}", this->ColMax, pos.Col()+textlength);
                std::cout << error << std::endl;
                throw std::invalid_argument(error);
            }
        }
        void ClearScreen();

        void CheckForKeyboardEventWorker()
        {
            uint iter = 1;
            while (this->IsRunning)
            {
                this->ScanKeyboardInput();

                std::this_thread::sleep_for(std::chrono::milliseconds(1));

                iter++;
            }
        }

        void FillScreen()
        {
            for (int y = 1; y < this->RowMax+1; y++)
            {
                //this->WriteToBuffer(std::string(this->ColMax, '*'), ScreenPos(0, y));
                this->WriteToBuffer(GetRenderingTableLookupKey(ScreenPos(1, y)), ScreenPos(1, y), 2);
            }
        }
        constexpr static std::string_view EmptyBuffer{};
    };
}

#endif //DUNGONSOFMORGINAR_NONBLOCKINGTERMINAL_HPP
