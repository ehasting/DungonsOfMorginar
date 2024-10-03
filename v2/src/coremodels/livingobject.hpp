//
// Created by eh on 1/9/22.
//

#ifndef DUNGONSOFMORGINAR_LIVINGOBJECT_HPP
#define DUNGONSOFMORGINAR_LIVINGOBJECT_HPP

#include "location.hpp"
#include "dynamicobject.hpp"
#include "locatedobject.hpp"
#include "core/livingstats.hpp"
#include <cmath>

namespace DofM
{
    class LivingObject : public LocatedObject
    {
    public:
        typedef std::shared_ptr<LivingObject> SLivingObject;
        LivingStats Stats;
        virtual const std::string GetDescriptionLine() = 0;
        std::vector<SLivingObject> LivingObjectsInRange;
        SLivingObject GetLivingObject()
        {
            return SLivingObject(this);
        }
        void UpsertLivingObjectIfInRange(SLivingObject objectToCheck)
        {
            if (this->ObjectLocation->IsBeside(objectToCheck->ObjectLocation))
            {
                if(std::find(this->LivingObjectsInRange.begin(), this->LivingObjectsInRange.end(), objectToCheck)
                    == this->LivingObjectsInRange.end())
                {
                    this->LivingObjectsInRange.push_back(objectToCheck);
                }
            }
        }

        void UpdateLivingObjectsInRange()
        {
            std::vector<SLivingObject> newlist;
            for (auto &obj : LivingObjectsInRange)
            {
                if (obj->ObjectLocation->IsBeside(this->ObjectLocation))
                {
                    newlist.push_back(obj);
                }
                else if (obj->ObjectLocation->IsOntop(this->ObjectLocation))
                {
                    newlist.push_back(obj);
                }
            }
            this->LivingObjectsInRange.clear();
            this->LivingObjectsInRange = newlist;
        }

        bool IsLocationEmpty(Location::SLocation futurelocation)
        {
            for (auto &obj : LivingObjectsInRange)
            {
                if (obj->ObjectLocation->IsSame(futurelocation))
                {
                    return false;
                }
            }
            return true;
        }
        bool TryMoveNorth()
        {
            Location::SLocation NewObjectLocation = std::make_shared<Location>(*ObjectLocation); NewObjectLocation->MoveNorth();
            if (CanMove() && this->ObjectMapRegion->CanMove(NewObjectLocation) && IsLocationEmpty(NewObjectLocation))
            {
                this->ObjectLocation->MoveNorth();
                Stats.Stamina.RemoveStats();
                return true;
            }
            return false;
        }
        bool TryMoveSouth()
        {
            Location::SLocation NewObjectLocation = std::make_shared<Location>(*ObjectLocation); NewObjectLocation->MoveSouth();
            if (CanMove() && this->ObjectMapRegion->CanMove(NewObjectLocation) && IsLocationEmpty(NewObjectLocation))
            {
                this->ObjectLocation->MoveSouth();
                Stats.Stamina.RemoveStats();
                return true;
            }
            return false;
        }
        bool TryMoveEast()
        {
            Location::SLocation NewObjectLocation = std::make_shared<Location>(*ObjectLocation); NewObjectLocation->MoveEast();
            if (CanMove() && this->ObjectMapRegion->CanMove(NewObjectLocation) && IsLocationEmpty(NewObjectLocation))
            {
                this->ObjectLocation->MoveEast();
                Stats.Stamina.RemoveStats();
                return true;
            }
            return false;
        }
        bool TryMoveWest()
        {
            Location::SLocation NewObjectLocation = std::make_shared<Location>(*ObjectLocation); NewObjectLocation->MoveWest();
            if (CanMove() && this->ObjectMapRegion->CanMove(NewObjectLocation) && IsLocationEmpty(NewObjectLocation))
            {
                this->ObjectLocation->MoveWest();
                Stats.Stamina.RemoveStats();
                return true;
            }
            return false;
        }
        void RegainStamina(int staminatoget = 1)
        {
            Stats.Stamina.AddStats(staminatoget);
        }
        bool IsAlive()
        {
            return Stats.Health.GetCurrent() > 0;
        }
    protected:
        LivingObject(std::string name, std::string tname, Location::SLocation location, DynamicObjectList dynobj)
            : LocatedObject(name, tname, location, dynobj)
        {
            Stats.Stamina.SetStats(30, 30);
            Stats.Health.SetStats(100,100);
        }
        virtual void Defence(int &dice, int &windice, int &damagereductionmin, int &damagereductionmax)
        {
            windice = 4;
            dice = 6;
            damagereductionmin = 1;
            damagereductionmax = 4;
        }


        bool CanMove()
        {
            return Stats.Stamina.GetCurrent() > 0;
        }

        int DefenceCheck()
        {
            int dice, windice, damagereductionmin, damagereductionmax = 0;
            this->Defence(dice,windice, damagereductionmin, damagereductionmax);
            if (this->LocalToolsObject.Dice(dice) >= windice)
            {
                return this->LocalToolsObject.Dice(damagereductionmax, damagereductionmin); // damage reduction;
            }
            return 0;
        }

        virtual void Attack(int &dice, int &windice, int &attackmin, int &attackmax)
        {
            windice = 4;
            dice = 6;
            attackmin = 1;
            attackmax = 4;
        }
        void AttackCheck(LivingObject &objectToAttack)
        {
            int dice, windice, attackmin, attackmax = 0;
            this->Attack(dice,windice, attackmin, attackmax);
            if (this->LocalToolsObject.Dice(dice) >= windice)
            {
                auto reduction = objectToAttack.DefenceCheck();
                auto attackscore = this->LocalToolsObject.Dice(attackmax, attackmin);
                if (reduction > attackscore)
                    reduction = attackscore;
                objectToAttack.Stats.Health.RemoveStats(attackscore - reduction);
            }
        }
    };
}

#endif //DUNGONSOFMORGINAR_LIVINGOBJECT_HPP
