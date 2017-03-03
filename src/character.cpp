#include "character.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>
#include "item.h"
#include <vector>
#include "asciirenderengine.h"

Character::Character()
{
    this->Name = "Unamed Hero";
    this->Dexterety = 0;
    this->Intelligence = 0;
    this->Strength = 0;
    this->GenerateCharacter();
}


void Character::GenerateCharacter()
{

    this->Dexterety = this->Dice(3, 12);
    this->Intelligence = this->Dice(3, 12);
    this->Strength = this->Dice(3, 12);

    this->Health = this->Dice(50,125*(this->Strength/2));
    this->Mana = this->Dice(50, (this->Intelligence*2)*12 );

    this->CurrentHealth = this->Health;
    this->CurrentMana = this->Mana;
}

void Character::ShowCharacterLine(AsciiRenderEngine &render, int lineoffset)
{

    render.Print("[" + this->Name + "] Health: " + std::to_string(this->CurrentHealth) + "/" + std::to_string(this->Health) +
                 " | Mana: " + std::to_string(this->CurrentMana) + "/"  + std::to_string(this->Mana) +
                 " | Str/Dex/Int: " + std::to_string(this->Strength) + "/" + std::to_string(this->Dexterety) + "/" + std::to_string(this->Intelligence),
                 AsciiRenderEngine::GREEN,
                 lineoffset);
}

void Character::ShowCharacter(AsciiRenderEngine &render, int lineoffset)
{
    render.Print("Name: " + this->Name, AsciiRenderEngine::BLUE, lineoffset++);
    render.Print("Health: " + std::to_string(this->Health) + " | Mana: " + std::to_string(this->Mana), AsciiRenderEngine::BLUE, lineoffset++);
    render.Print("Str/Dex/Int: " + std::to_string(this->Strength) + "/" + std::to_string(this->Dexterety) + "/" + std::to_string(this->Intelligence), AsciiRenderEngine::BLUE, lineoffset++);
    this->ShowInventory(render, lineoffset++);
}

void Character::ShowInventory(AsciiRenderEngine &render, int lineoffset)
{
    render.Print("Inventory:", AsciiRenderEngine::WHITE, lineoffset++);
    for (auto inv : this->Inventory)
    {
        render.Print(" * " + inv->Name + " - " + inv->Description, AsciiRenderEngine::BLUE, lineoffset++);
    }
}

int Character::Dice(int min, int max)
{

    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
    int retvalue = dist6(this->rng);

    //std::cout << "min("<< min << ") " << "max("<< max << ") rnd: " << retvalue << std::endl;

    return retvalue;
}
