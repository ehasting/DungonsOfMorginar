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

namespace DofM
{
    class Mouse : public LivingObject
    {
    public:
        typedef std::shared_ptr<Mouse> SMouse;
        SMouse GetMouse()
        {
            return SMouse(this);
        }
        inline static const std::string TypeName = "mouse";
        enum MouseStates
        {
            FIGHTING,
            MOVING,
            RESTING,
            DEAD
        };
        Mouse(std::string name, Location::SLocation loc, DynamicObjectList dynobj)
                : LivingObject(name, TypeName, loc, dynobj)
        {
            SetState(MouseStates::MOVING);
        }
        MouseStates State;
        int ReactEveryTick = 2;
        int MoveEveryTick = 2;
        std::string CurrentDescription = "";

        void SetState(MouseStates newstate)
        {
            std::string stateword = "";
            switch (newstate)
            {
                case FIGHTING:
                    stateword = "FIGHTING";
                    break;
                case MOVING:
                    stateword = "MOVING";
                    break;
                case RESTING:
                    stateword = "RESTING";
                    break;
                case DEAD:
                    stateword = "DEAD";
                    break;
            }
            std::cout << fmt::format("Changing state to {} ({})\n", newstate, stateword);
            State = newstate;
        }

        void RunISM(long long int tick)
        {

            switch (this->State)
            {
                case MouseStates::MOVING:
                    Move(tick);
                    if (this->Stats.Stamina.GetPrecentage() < 5)
                    {
                        SetState(MouseStates::RESTING);
                    }
                    break;
                case MouseStates::RESTING:
                    this->RegainStamina();
                    if (this->Stats.Stamina.GetPrecentage() > 50)
                    {
                        SetState(MouseStates::MOVING);
                    }
                    break;
                case MouseStates::FIGHTING:
                    this->Fight(tick);
                    break;
                case MouseStates::DEAD:
                    this->CurrentDescription = fmt::format("[{}] {} is dead (Loc: {})", this->TypeName, UniqueName, Stats.Stamina.GetCurrent(), Stats.Health.GetCurrent(), this->ObjectLocation->ToString());
                    break;
            }
        }

        void Move(long long int tick)
        {
            if ((tick % MoveEveryTick) == 0)
            {
                auto dice = this->LocalToolsObject.Dice(50,1);

                if (dice >=1 && dice <= 10)
                {
                    if (this->TryMoveNorth())
                    {
                        return;
                    }
                }
                else if (dice >=11 && dice <= 20)
                {
                    if (this->TryMoveSouth())
                    {
                        return;
                    }
                }
                else if (dice >=21 && dice <= 30)
                {
                    if (this->TryMoveEast())
                    {
                        return;
                    }
                }
                else if (dice >=41 && dice <= 50)
                {
                    if (this->TryMoveWest())
                    {
                        return;
                    }
                }
            }
        }

        void CheckForFights(long long int tick)
        {
            for (auto &n:*this->DynamicObjects)
            {
                if (this->IsSameObject(*n))
                {
                    continue;
                }
                auto tn = n->GetTypeName();
                if (tn == Character::TypeName)
                {

                }
                else if (tn == Mouse::TypeName)
                {

                }
            }
        }

        void CheckForFoesNairby(long long int tick)
        {
            this->UpdateLivingObjectsInRange();
            for (auto &n:*this->DynamicObjects)
            {
                if (this->IsSameObject(*n))
                {
                    continue;
                }
                auto tn = n->GetTypeName();
                if (tn == Character::TypeName)
                {

                }
                else if (tn == Mouse::TypeName)
                {
                    auto currenttarget = std::dynamic_pointer_cast<Mouse>(n);
                    if (this->ObjectLocation->ToString() == currenttarget->ObjectLocation->ToString())
                    {
                        //std::cout  << fmt::format("Same location?!  {} {} \n", this->UniqueName, currenttarget->UniqueName);
                    }
                    this->UpsertLivingObjectIfInRange(currenttarget);
                }
            }
        }

        void Fight(long long int tick)
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
                        auto currenttarget = std::dynamic_pointer_cast<Mouse>(n);
                        //auto currenttarget = n->GetRealObject<Mouse>();
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

        virtual bool Update(long long int tick)
        {
            CheckForFoesNairby(tick);
            if ((tick % ReactEveryTick) == 0)
            {
                RunISM(tick);
            }
            else
            {
                this->CurrentDescription = fmt::format("[{}] {} ({})  {} / {} (HP / ST) (Loc: {})", this->TypeName, UniqueName, UniqueNameHash, Stats.Health.GetCurrent(), Stats.Stamina.GetCurrent(), this->ObjectLocation->ToString());
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
