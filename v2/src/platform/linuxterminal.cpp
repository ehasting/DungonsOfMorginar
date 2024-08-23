//
// Created by EgilHasting on 25/09/2022.
//

#include "linuxterminal.hpp"
#include <termios.h>

namespace DofM
{
    void LinuxTerminal::ScanKeyboardInput()
    {
        SDL_Event event;

        // Events management
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    this->Terminate();
                    break;
                case SDL_TEXTINPUT:
                {
                    std::string newtext(event.text.text);
                    this->AddToTextBuffer(newtext);
                }
                    break;
                case SDL_TEXTEDITING:
                    /*
                    Update the composition text.
                    Update the cursor position.
                    Update the selection length (if any).
                    */
                    //composition = event.edit.text;
                    //cursor = event.edit.start;
                    //selection_len = event.edit.length;
                    break;
                case SDL_KEYDOWN:
                    this->AddEventToQueue(event.key.keysym.scancode);
                    break;
            }
        }
     
    }
} // DofM