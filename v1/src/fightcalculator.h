#ifndef FIGHTCALCULATOR_H
#define FIGHTCALCULATOR_H
#include "character.h"
#include "tools.h"
class FightCalculator
{
public:
    FightCalculator();

    int CalculatePower(Character &thething, bool defender)
    {
        float defender_mod = 1.02f;
        if (defender)
            return thething.Intelligence * thething.Strength * thething.Dexterety * defender_mod;
        else
            return thething.Intelligence * thething.Strength * thething.Dexterety;
    }

    std::string Crunch(Character &Attacker, Character &Defender)
    {
        int att_power = this->CalculatePower(Attacker, false);
        int def_power = this->CalculatePower(Defender, true);

        int damage_to_def = (int)(Tools::getInstance().Dice( ((int)att_power*0.2f),  att_power) / 10.0f);

        Defender.CurrentHealth = Defender.CurrentHealth - damage_to_def;
        return Attacker.Name + " damage " +Defender.Name + " by " + std::to_string(damage_to_def) + " HP";
    }
};

#endif // FIGHTCALCULATOR_H
