//
// Created by eh on 1/5/22.
//

#ifndef DUNGONSOFMORGINAR_MOUSE_HPP
#define DUNGONSOFMORGINAR_MOUSE_HPP
#include "coremodels/dynamicobject.hpp"
#include "coremodels/livingobject.hpp"
#include "coremodels/location.hpp"
#include "objects/character.hpp"
#include "fmt/core.h"
#include <string>
#include <memory>
#include <iostream>

namespace DofM
{
    class Mouse : public LivingObject
    {
    public:
        inline static const std::string TypeName = "mouse";

        Mouse(std::string name, Location::SLocation loc, DynamicObjectList dynobj)
                : LivingObject(name, TypeName, loc, dynobj)
        {
        }

        int ReactEveryTick = 2;
        int MoveEveryTick = 2;
        std::string CurrentDescription = "";

        virtual bool Update(long long int tick)
        {
            if (!this->IsAlive())
            {
                this->CurrentDescription = fmt::format("[{}] {} is dead (Loc: {})", this->TypeName, UniqueName, Stats.Stamina.GetCurrent(), Stats.Health.GetCurrent(), this->ObjectLocation->ToString());
                return true;
            }

            bool HasMoved = false;
            if ((tick % MoveEveryTick) == 0)
            {
                if (this->LocalToolsObject.Dice(6) > 4 && !this->ObjectMapRegion->IsAtNorthWall(this->ObjectLocation))
                {
                    if (this->TryMoveNorth())
                    {
                        HasMoved = true;
                    }
                }
                else if (this->LocalToolsObject.Dice(6) > 4 && !this->ObjectMapRegion->IsAtSouthWall(this->ObjectLocation))
                {
                    if (this->TryMoveSouth())
                    {
                        HasMoved = true;
                    }
                }
                else if (this->LocalToolsObject.Dice(6) > 4 && !this->ObjectMapRegion->IsAtEastWall(this->ObjectLocation))
                {
                    if (this->TryMoveEast())
                    {
                        HasMoved = true;
                    }
                }
                else if (this->LocalToolsObject.Dice(6) > 4 && !this->ObjectMapRegion->IsAtWestWall(this->ObjectLocation))
                {
                    if (this->TryMoveWest())
                    {
                        HasMoved = true;
                    }
                }

                if (!HasMoved)
                {
                    this->RegainStamina();
                }
            }

            if ((tick % ReactEveryTick) == 0)
            {
                //this->CurrentDescription = fmt::format("Looking for someone to attack (tick: {})", tick);
                for (auto &n:*this->DynamicObjects)
                {
                    if (this->IsSameObject(*n))
                    {
                        //std::cout << fmt::format("FOUND Self, ignoring ({})", this->UniqueName) << std::endl;
                    }
                    else if (n->GetTypeName() == Character::TypeName)
                    {
                        auto currenttarget = n->GetRealObject<Character>();
                        if (this->ObjectLocation->IsBeside(currenttarget->ObjectLocation))
                        {
                            if (this->LocalToolsObject.Dice(6) >= 4)
                            {
                                this->CurrentDescription.append(fmt::format(", attacking: {}", currenttarget->UniqueName));
                                this->AttackCheck(*currenttarget);
                            }
                        }
                    }
                    else if (n->GetTypeName() == Mouse::TypeName)
                    {
                        auto currenttarget = n->GetRealObject<Mouse>();
                        if (!currenttarget->IsAlive())
                        {
                            // Its dead, so no point of updating it
                            continue;
                        }
                        if (this->ObjectLocation->IsBeside(currenttarget->ObjectLocation))
                        {
                            if (this->LocalToolsObject.Dice(6) >= 4)
                            {
                                this->CurrentDescription.append(fmt::format(", attacking: {}", currenttarget->UniqueName));
                                this->AttackCheck(*currenttarget);
                            }
                        }
                    }
                }
            }
            else
            {
                this->CurrentDescription = fmt::format("[{}] {}  {} / {} (HP / ST) (Loc: {})", this->TypeName, UniqueName, Stats.Health.GetCurrent(), Stats.Stamina.GetCurrent(), this->ObjectLocation->ToString());
            }

            return true;
        };

        const std::string GetDescriptionLine()
        {
            return CurrentDescription;
        }
    };
}

#endif //DUNGONSOFMORGINAR_MOUSE_HPP
