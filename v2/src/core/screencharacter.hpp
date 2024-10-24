#ifndef SCREENCHARACTER_H
#define SCREENCHARACTER_H
#include <SDL2/SDL.h>
#include <string>
#include <fmt/core.h>

namespace DofM
{
constexpr uint64_t NO_HASH = 0;
constexpr uint8_t b1 = 8 * 7;
constexpr uint8_t b2 = 8 * 6;
constexpr uint8_t b3 = 8 * 5;
constexpr uint8_t b4 = 8 * 4;
constexpr uint8_t b5 = 8 * 3;
constexpr uint8_t b6 = 8 * 2;
constexpr uint8_t b7 = 8 * 1;
constexpr uint8_t b8 = 0;
constexpr SDL_Color White = {255, 255, 255, 255};
constexpr SDL_Color Black = {0, 0, 0, 255};

class ScreenCharacter
{
private:

    int Priority;
    uint64_t UniqueKeyCache = NO_HASH;
public:
    std::string Character;
    SDL_Color Color;

    static int DefaultPriority;
    ScreenCharacter()
    {
        Reset();
    }
    ScreenCharacter(std::string ch, SDL_Color color = White, int pri = DefaultPriority)
    {
        this->SetScreenCharacter(ch, color, pri);
    }
    ~ScreenCharacter()
    {

    }
    void SetScreenCharacter(std::string ch, SDL_Color color, int pri = DefaultPriority)
    {
        this->Character = ch;
        this->Color = color;
        this->Priority = pri;
        this->UniqueKeyCache = NO_HASH;
    }

    bool IsPrioritized(int currentpri)
    {
        //fmt::print("{} <= {}\n", this->Priority, currentpri);
        if (this->Priority == -1)
        {
            return false;
        }
        return this->Priority <= currentpri ? true : false;
    }

    void Reset()
    {
        this->SetScreenCharacter(" ", White, DefaultPriority);
    }

    void FlagAsUsed()
    {
        this->Priority = -1;
    }
    uint64_t CreateKey()
    {
        if (this->UniqueKeyCache != NO_HASH)
        {
            return this->UniqueKeyCache;
        }
        uint64_t rval = 0ULL;
        switch (Character.size())
        {
        case 0:
            break;
        case 1:
            rval |= (uint64_t)Character.at(0) << b1;
            break;
        case 2:
            rval |= (uint64_t)Character.at(0) << b1;
            rval |= (uint64_t)Character.at(1) << b2;
            break;
        case 3:
            rval |= (uint64_t)Character.at(0) << b1;
            rval |= (uint64_t)Character.at(1) << b2;
            rval |= (uint64_t)Character.at(2) << b3;
            break;
        case 4:
            rval |= (uint64_t)Character.at(0) << b1;
            rval |= (uint64_t)Character.at(1) << b2;
            rval |= (uint64_t)Character.at(2) << b3;
            rval |= (uint64_t)Character.at(3) << b4;
            break;
        }
        rval |= (uint64_t)Color.a << b5;
        rval |= (uint64_t)Color.b << b6;
        rval |= (uint64_t)Color.g << b7;
        rval |= (uint64_t)Color.r << b8;
        this->UniqueKeyCache = rval;
        return rval;
    }
};
}
#endif // SCREENCHARACTER_H
