#ifndef HERO_H
#define HERO_H
#include "character.h"

class Hero : public Character
{
public:
    Hero();
    std::vector<Item*> Inventory;
    void ShowCharacter(AsciiRenderEngine &render, int &lineoffset);
    void ShowCharacterLine(AsciiRenderEngine &render, int &lineoffset);
    void ShowInventory(AsciiRenderEngine &render, int &lineoffset);
    long Experience;
};

#endif // HERO_H
