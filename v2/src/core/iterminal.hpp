//
// Created by EgilHasting on 25/09/2022.
//

#ifndef DUNGONSOFMORGINAR_ITERMINAL_HPP
#define DUNGONSOFMORGINAR_ITERMINAL_HPP
#include <tuple>
#include <memory>
#include <vector>
#include <queue>
#include <SDL2/SDL.h>

namespace DofM
{

    class ITerminal
    {
    public:
        ITerminal() { }
        ~ITerminal() { }
        virtual void SetupNonBlockingTerminal() = 0;
        virtual void ClearScreen() = 0;
        virtual void UpdateScreen() = 0;
        virtual void ScanKeyboardInput() = 0;
        virtual void ReadPlatformNativeTerminalSize(unsigned short &maxrow, unsigned short &maxcol) = 0;
        virtual void PrintText(int x, int y, std::string &text, SDL_Color fgcolor) = 0;
        virtual void PrintTextCopy(int x, int y, std::string text, SDL_Color fgcolor)
        {
            std::string tmp = text;
            this->PrintText(x, y, tmp, fgcolor);
        };
        virtual void PrintChar(int x, int y, char &text) = 0;
        bool IsTerminated = false;
        virtual void Terminate()
        {
            this->IsTerminated = true;
        }
        std::mutex TextBufferMutex;
        std::string TextBuffer;
        void AddToTextBuffer(std::string text)
        {
            TextBufferMutex.lock();
            this->TextBuffer.append(text);
            TextBufferMutex.unlock();
        }
        std::string GetPendingText()
        {
            TextBufferMutex.lock();
            std::string rval = this->TextBuffer;
            this->TextBuffer.clear();
            TextBufferMutex.unlock();
            return rval;
        }

        std::mutex EventListMutext;
        std::queue<SDL_Scancode> EventList;
        void AddEventToQueue(SDL_Scancode event)
        {
            EventListMutext.lock();
            this->EventList.push(event);
            EventListMutext.unlock();
        }
        std::vector<SDL_Scancode> GetPendingEvents()
        {
            std::vector<SDL_Scancode> rval;
            EventListMutext.lock();
            while (!EventList.empty())
            {
                rval.push_back(EventList.front());
                EventList.pop();
            }
            EventListMutext.unlock();
            return rval;
        }
    };

} // DofM

#endif //DUNGONSOFMORGINAR_ITERMINAL_HPP
