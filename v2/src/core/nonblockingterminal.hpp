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

#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <queue>
#include <thread>
#include "screenpos.hpp"
#include "keycodes.hpp"
#include "inputhandler.hpp"
#include "iterminal.hpp"

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
    const SDL_Color White = {255, 255, 255, 255};
    class ScreenCharacter
    {
    public:
        std::string Character;
        SDL_Color Color;
        int Priority = 10;
        ScreenCharacter()
        {
            Reset();
        }
        ScreenCharacter(std::string ch, SDL_Color color, int pri = 10)
        {
            this->Character = ch;
            this->Color = color;
            this->Priority = pri;
        }
        void Reset()
        {
            this->Character = " ";
            this->Color = White;
            this->Priority = 10;
        }

    };
    class NonBlockingTerminal
    {
    public:
        NonBlockingTerminal(std::shared_ptr<ITerminal> terminaltype);
        ~NonBlockingTerminal();
        void Terminate()
        {
            this->IsRunning = false;
        }


    protected:
        bool IsReady = false;
        bool IsRunning = true;
        bool IsInNonBlockingMode = false;



        std::shared_ptr<ITerminal> Terminal;

        void ProcessKeyPressEventQueue();

        unsigned int ScreenBufferLength;
        std::vector<ScreenCharacter> ScreenBuffer;
        std::vector<ScreenCharacter> DrawScreenBuffer;

        void ResizeScreenBuffer()
        {
            this->ScreenBufferLength = this->RowMax * this->ColMax;
            this->ScreenBuffer.clear();
            this->ScreenBuffer.resize(this->ScreenBufferLength);
            for(int x = 0; x >this->ScreenBufferLength; x++) this->ScreenBuffer.push_back(ScreenCharacter());
        }

        void ClearScreenBuffer()
        {
            for(auto &n : this->ScreenBuffer)
            {
                n.Reset();
            }
        }

        unsigned int GetBufferPosition(ScreenPos pos)
        {
            unsigned int rval = ((pos.Row() * this->ColMax) + pos.Col());
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
            pos.SetRow(row);
            pos.SetCol(col);
            return pos;
        }

        unsigned int RedrawCounter = 0;
        std::mutex DrawLock;

    public:
        std::mutex& GetLock()
        {
            return this->DrawLock;
        }

        unsigned short int RowMax = USHRT_MAX;
        unsigned short int ColMax = USHRT_MAX;

        void ReadTerminalSize();

        void FlipDrawbuffer();

        void Redraw();

        void WriteToBuffer(std::string text, ScreenPos pos, SDL_Color fg = White , int priority = 10);

        void CheckIfOnScreen(ScreenPos pos, unsigned int textlength)
        {
            if (pos.Col() > this->ColMax)
            {
                //fmt::print("col out of bounds (max: {}, got: {})", this->ColMax, pos.Col());
                throw std::invalid_argument("See last printout");
            }
            if (pos.Row() > this->RowMax)
            {
                //fmt::print("row out of bounds (max: {}, got: {})", this->RowMax, pos.Row());
                throw std::invalid_argument("See last printout");
            }
            if (pos.Col()+textlength > this->ColMax)
            {
                //fmt::print("textlength out of bounds (max: {}, got: {})", this->ColMax, pos.Col()+textlength);
                //throw std::invalid_argument(error);
            }
        }


        constexpr static std::string_view EmptyBuffer{};
    };
}

#endif //DUNGONSOFMORGINAR_NONBLOCKINGTERMINAL_HPP
