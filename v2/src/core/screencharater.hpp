#ifndef SCREENCHARATER_H
#define SCREENCHARATER_H
#include <SDL2/SDL.h>
#include <string>

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
}
#endif // SCREENCHARATER_H
