/*
MIT License

Copyright (c) 2017 Egil Hasting

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
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
    this->Moves = 0;
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



int Character::Dice(int min, int max)
{

    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
    int retvalue = dist6(this->rng);

    //std::cout << "min("<< min << ") " << "max("<< max << ") rnd: " << retvalue << std::endl;

    return retvalue;
}
