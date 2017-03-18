#include "hero.h"

Hero::Hero() : Character()
{
    this->Experience = 0;
}

void Hero::ShowCharacterLine(AsciiRenderEngine &render, int &lineoffset)
{
    render.Print(this->GetCharacterString(), RenderEngine::GREEN, lineoffset);
}

void Hero::ShowCharacter(AsciiRenderEngine &render, int &lineoffset)
{
    render.Print("Name: " + this->Name, RenderEngine::BLUE, lineoffset);
    lineoffset++;
    render.Print("Health: " + std::to_string(this->Health) + " | Mana: " + std::to_string(this->Mana), RenderEngine::BLUE, lineoffset);
    lineoffset++;
    render.Print("Str/Dex/Int: " + std::to_string(this->Strength) + "/" + std::to_string(this->Dexterety) + "/" + std::to_string(this->Intelligence), RenderEngine::BLUE, lineoffset);
    lineoffset++;
    this->ShowInventory(render, lineoffset);
}

void Hero::ShowInventory(AsciiRenderEngine &render, int &lineoffset)
{
    render.Print("Inventory:", RenderEngine::WHITE, lineoffset);
    lineoffset++;
    for (auto inv : this->Inventory)
    {
        render.Print(" * " + inv->Name + " - " + inv->Description, RenderEngine::BLUE, lineoffset);
        lineoffset++;
    }
}
