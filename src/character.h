#ifndef CHARACTER_H
#define CHARACTER_H
#include <random>
#include "item.h"
#include <vector>
#include "asciirenderengine.h"

class Character
{
private:
    int Dice(int min, int max);
    std::mt19937 rng;
public:
    Character();
    void GenerateCharacter();
    std::string Name;
    int Health;
    int CurrentHealth;
    int Mana;
    int CurrentMana;
    int Dexterety;
    int Strength;
    int Intelligence;
    std::vector<Item*> Inventory;
    void ShowCharacter(AsciiRenderEngine &render, int lineoffset);
    void ShowCharacterLine(AsciiRenderEngine &render, int lineoffset);
    void ShowInventory(AsciiRenderEngine &render, int lineoffset);
};

#endif // CHARACTER_H
