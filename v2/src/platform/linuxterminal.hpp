//
// Created by EgilHasting on 25/09/2022.
//

#ifndef DUNGONSOFMORGINAR_LINUXTERMINAL_HPP
#define DUNGONSOFMORGINAR_LINUXTERMINAL_HPP
#include "core/iterminal.hpp"
#include <utility>
#include <iostream>
#include <unordered_map>
#include <string>
#include <chrono>
#include <sys/ioctl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "core/DofM_TextRenderCache.h"


namespace DofM
{

    class LinuxTerminal : public ITerminal
    {


    public:
        LinuxTerminal() : ITerminal()
        {
            SDL_Init(SDL_INIT_VIDEO);
            std::cout << this->ScreenWidth << " x " << this->ScreenHeight <<std::endl;
            SDL_CreateWindowAndRenderer(this->ScreenWidth, this->ScreenHeight, 0, &window, &renderer);
            TTF_Init();
            font = TTF_OpenFont("resources/dos.ttf", this->FontHeight);
        }
        ~LinuxTerminal()
        {
            TTF_Quit();
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
        void ScanKeyboardInput() override;
        void SetupNonBlockingTerminal() override
        {
            this->ClearScreen();
        }

        void ReadPlatformNativeTerminalSize(unsigned short &maxrow, unsigned short &maxcol) override
        {
            maxrow = this->ScreenHeight / this->FontHeight;
            maxcol = this->ScreenWidth / this->FontWidth;
            //std::cout << "Row: " << maxrow << " Col: " << maxcol << std::endl;
        }
        void ClearScreen() override
        {
            SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
            SDL_RenderClear(this->renderer);
        }
        void UpdateScreen() override
        {
            SDL_RenderPresent(this->renderer);
        }

        void PrintLetter(int x, int y, Uint32 letter, SDL_Color fgcolor) override
        {
            if (y * FontWidth > this->ScreenWidth)
            {
                std::cout << fmt::format("Error letter on (x / y): {} / {} = {} ", x, y, letter) << std::endl;
                std::cout << fmt::format("String length is {} which is too long for max width on {}", y * FontWidth, this->ScreenWidth) << std::endl;
                exit(1);
            }
            auto texture = RenderCache.GetTexture(letter, this->font, this->renderer, fgcolor);
            //std::cout << fmt::format("{}{}{}{}_{}", fgcolor.r, fgcolor.g, fgcolor.b, fgcolor.a, text.c_str()) << std::endl;

            SDL_Rect src = {0, 0, FontWidth, FontHeight};
            SDL_Rect dest = {x  * FontWidth, y * FontHeight, FontWidth, FontHeight};
            SDL_RenderCopy(renderer, texture, &src, &dest);
        }
        void PrintChar(int x, int y, char &text) override
        {

        }
    private:
        char ReadCharBuffer[2];
        SDL_Window *window;
        SDL_Renderer *renderer;
        TTF_Font *font;

        DofM_TextRenderCache RenderCache;

        const unsigned short FontHeight = 16;
        const unsigned short FontWidth = 9;
        const unsigned short ScreenHeight = 720;
        const unsigned short ScreenWidth = 1280;

    };

} // DofM

#endif //DUNGONSOFMORGINAR_LINUXTERMINAL_HPP
