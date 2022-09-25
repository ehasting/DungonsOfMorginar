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
    class NonBlockingTerminal
    {
    public:
        NonBlockingTerminal(std::shared_ptr<ITerminal> terminaltype, std::shared_ptr<InputHandler> inhandler);
        ~NonBlockingTerminal();
        void Terminate()
        {
            this->IsRunning = false;
        }
    protected:
        bool IsReady = false;
        bool IsRunning = true;
        bool IsInNonBlockingMode = false;
        std::queue<char> InputQueue;

        std::mutex IOMutex;
        std::mutex DrawMutex;

        std::shared_ptr<InputHandler> InHandler;
        std::shared_ptr<ITerminal> Terminal;

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
        unsigned short int RowMax = USHRT_MAX;
        unsigned short int ColMax = USHRT_MAX;

        void ReadTerminalSize();

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


        void CheckForKeyboardEventWorker()
        {
            unsigned int iter = 1;
            std::shared_ptr<std::vector<char> > outdata = std::make_shared<std::vector<char> >();
            outdata->reserve(16);
            while (this->IsRunning)
            {
                this->Terminal->ScanKeyboardInput(outdata);
                if (!outdata->empty())
                {
                    this->IOMutex.lock();
                    for ( auto &n : *outdata)
                    {
                        this->InputQueue.push(n);
                    }
                    this->IOMutex.unlock();
                    this->ProcessKeyPressEventQueue();
                }


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
