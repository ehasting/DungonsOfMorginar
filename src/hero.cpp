#include "hero.h"

Hero::Hero() : Character()
{
    this->Experience = 0;
}

void Hero::ShowCharacterLine(AsciiRenderEngine &render, int lineoffset)
{

    render.Print("[" + this->Name + "] Health: " + std::to_string(this->CurrentHealth) + "/" + std::to_string(this->Health) +
                 " | Mana: " + std::to_string(this->CurrentMana) + "/"  + std::to_string(this->Mana) +
                 " | Str/Dex/Int: " + std::to_string(this->Strength) + "/" + std::to_string(this->Dexterety) + "/" + std::to_string(this->Intelligence) + "  Moves: " + std::to_string(this->Moves),
                 RenderEngine::GREEN,
                 lineoffset);
}

void Hero::ShowCharacter(AsciiRenderEngine &render, int lineoffset)
{
    render.Print("Name: " + this->Name, RenderEngine::BLUE, lineoffset++);
    render.Print("Health: " + std::to_string(this->Health) + " | Mana: " + std::to_string(this->Mana), RenderEngine::BLUE, lineoffset++);
    render.Print("Str/Dex/Int: " + std::to_string(this->Strength) + "/" + std::to_string(this->Dexterety) + "/" + std::to_string(this->Intelligence), RenderEngine::BLUE, lineoffset++);
    this->ShowInventory(render, lineoffset++);
}

void Hero::ShowInventory(AsciiRenderEngine &render, int lineoffset)
{
    render.Print("Inventory:", RenderEngine::WHITE, lineoffset++);
    for (auto inv : this->Inventory)
    {
        render.Print(" * " + inv->Name + " - " + inv->Description, RenderEngine::BLUE, lineoffset++);
    }
}
