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
#include "thegame.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "promptparser.h"
#include "locations.h"
#include "location.h"
#include "item.h"
#include "tools.h"
#include "action.h"
#include "asciirenderengine.h"
#include "monsters.h"
#include "fightcalculator.h"

TheGame::TheGame()
{
    this->running = true;
    this->mobs = new Monsters(10);
}


void TheGame::SetupCommands(int &printcursor)
{
    Action actionExit("exit");
    actionExit.ActionFunction = [&] {
        this->running = false;
    };
    this->prompt.Actions.push_back(actionExit);

    Action actionTake("take");
    actionTake.ActionFunction = [&] {
        for (int itemcnt = 0; itemcnt < this->loc.CurrentRoom->ItemList.size(); itemcnt++)
        {
            if (this->loc.CurrentRoom->ItemList.at(itemcnt)->Name == this->prompt.GetCommandList().at(1))
            {
                this->hero.Inventory.push_back(this->loc.CurrentRoom->ItemList.at(itemcnt));
                this->loc.CurrentRoom->ItemList.erase(this->loc.CurrentRoom->ItemList.begin()+itemcnt);
                return;
            }
        }
        // trying now to - contains match
        for (int itemcnt = 0; itemcnt < this->loc.CurrentRoom->ItemList.size(); itemcnt++)
        {
            if (this->loc.CurrentRoom->ItemList.at(itemcnt)->Name.find(this->prompt.GetCommandList().at(1)) != std::string::npos)
            {
                this->hero.Inventory.push_back(this->loc.CurrentRoom->ItemList.at(itemcnt));
                this->loc.CurrentRoom->ItemList.erase(this->loc.CurrentRoom->ItemList.begin()+itemcnt);
                return;
            }
        }
    };
    this->prompt.Actions.push_back(actionTake);

    Action actionUse("use");
    actionUse.ActionFunction = [&] {
        for (int herocnt = 0; herocnt < this->hero.Inventory.size(); herocnt++)
        {
            if (this->hero.Inventory.at(herocnt)->Name == this->prompt.GetCommandList().at(1))
            {
                Direction::Directions deleteKey;
                for (auto ex : this->loc.CurrentRoom->Blockers)
                {
                    if (ex.second->Name == this->prompt.GetCommandList().at(1))
                    {
                        deleteKey = ex.first;
                        //std::cout << "You used " << this->prompt.GetCommandList().at(1) << std::endl;
                        break;
                    }
                }
                this->loc.CurrentRoom->Blockers.erase(deleteKey);
            }
        }
    };
    this->prompt.Actions.push_back(actionUse);

    Action actionDrop("drop");
    actionDrop.ActionFunction = [&] {
        for (int herocnt = 0; herocnt < this->hero.Inventory.size(); herocnt++)
        {
            if (this->hero.Inventory.at(herocnt)->Name == this->prompt.GetCommandList().at(1))
            {
                this->loc.CurrentRoom->ItemList.push_back(this->hero.Inventory.at(herocnt));
                this->hero.Inventory.erase(this->hero.Inventory.begin()+herocnt);
            }
        }
    };
    this->prompt.Actions.push_back(actionDrop);

    Action actionGo("go");
    actionGo.ActionFunction = [&] {
        if (this->prompt.GetCommandList().size() == 1)
            return;
        Direction::Directions input_direction = Direction::from_string(this->prompt.GetCommandList().at(1));
        if (this->loc.FindNewRoom(input_direction))
        {
            this->hero.Moves++;
            for (int x = 0; x < this->mobs->TheMonsters.size(); x++)
            {
                if(this->mobs->TheMonsters[x].ShouldIMove())
                {
                    this->mobs->TheMonsters[x].MoveMosterRandom(this->loc.Map);
                }
            }
        }
    };
    this->prompt.Actions.push_back(actionGo);

    Action actionAttack("attack");
    actionAttack.ActionFunction = [&] {
        std::vector<Monster*> CurrentMonsters = this->mobs->GetMonsters(this->loc.CurrentRoom->Coords);
        if (CurrentMonsters.size() ==  0)
        {
           this->render.Print("There are nothing to attack...", RenderEngine::COLOR::WHITE, printcursor, 0);
        }
        FightCalculator calc;
        int monsterindextoattack = Tools::getInstance().Dice(0, CurrentMonsters.size()-1);
        std::string attacktxt = calc.Crunch(this->hero, *CurrentMonsters.at(monsterindextoattack));
        this->render.Print(attacktxt, CurrentMonsters.at(monsterindextoattack)->Color, printcursor, 2);
        if (Tools::getInstance().Dice(1,20) > 10)
        {
            if (!CurrentMonsters.at(monsterindextoattack)->IsDead())
            {
                this->render.Print("A counter attack is launched", CurrentMonsters.at(monsterindextoattack)->Color, printcursor,0);
                std::string counterattacktxt = calc.Crunch(*CurrentMonsters.at(monsterindextoattack), this->hero);
                this->render.Print(counterattacktxt, CurrentMonsters.at(monsterindextoattack)->Color, printcursor, 2);
            }
        }
        else
        {
            this->render.Print(CurrentMonsters.at(monsterindextoattack)->Name + " is without any defense...", CurrentMonsters.at(monsterindextoattack)->Color, printcursor,0);
        }


    };
    this->prompt.Actions.push_back(actionAttack);

}

void TheGame::RecalculateScreen()
{
    if (this->render.IsTerminalSizeChanged())
    {
        this->render = AsciiRenderEngine();
    }
}

void TheGame::Run()
{

    //std::cout << "The Game" << std::endl;
    /*
    Item KeyToEntrance(1, "dirtykey", "laying on the floor under some papers", "this key seems to belonging to an old lock");


    Location DarkRoom("DarkRoom", "Dark room, light is comming from west through a small key hole. Also an old window is giving a gray dim light to the floor.");
    Location EntranceHall("EntranceHall", "Large entrance hall with an entrance door at the end");
    Location OutsideHouse("OutsideHouse", "A large garden outside");
    Location DriveWay("DriveWay", "There is a driveway leading toward the main road.");
    Location GardenFountain("GardenFountain", "A beautiful fountain is in the middle of a large garden, surrounded by a tall hedge");

    DarkRoom.Linked["west"] = &EntranceHall;
    DarkRoom.Blockers["west"] = &KeyToEntrance;
    EntranceHall.Linked["east"] = &DarkRoom;
    EntranceHall.Linked["west"] = &OutsideHouse;
    OutsideHouse.Linked["east"] = &EntranceHall;
    OutsideHouse.Linked["south"] = &GardenFountain;
    OutsideHouse.Linked["north"] = &DriveWay;

    GardenFountain.Linked["north"] = &OutsideHouse;

    DriveWay.Linked["south"] = &OutsideHouse;


    DarkRoom.ItemList.push_back(&KeyToEntrance);

    this->loc.Map.push_back(DarkRoom);
    this->loc.Map.push_back(EntranceHall);
    this->loc.Map.push_back(OutsideHouse);
    this->loc.Map.push_back(DriveWay);
    this->loc.Map.push_back(GardenFountain);
    */


    int n = 0;

    bool IsMoreMonsters = true;
    for (auto n : this->maze.TheMaze)
    {
        this->loc.Map.push_back(n.second);
        while (IsMoreMonsters && (Tools::getInstance().Dice(1,20) > 10))
        {
            IsMoreMonsters = this->mobs->GenerateNextMonster(n.second.Coords);
        }
    }
    this->loc.Init();

    this->SetupCommands(n);

    render.ClearScreen();
    while(this->running)
    {
        n = 0;
        this->RecalculateScreen();
        this->hero.ShowCharacterLine(render, n);
        render.Print(std::string(render.GetLastChar()-1, '-'), RenderEngine::WHITE, n);
        render.Print("Visited Rooms: " + std::to_string(this->loc.Visited.size()) + " of "+ std::to_string(this->loc.Map.size()) +" rooms, (you said: " +  prompt.GetCommands() + ")", RenderEngine::GREEN, n);
        n++; // Space
        this->loc.CurrentRoom->ShowRoom(render, n);
        n++; // Space
        this->hero.ShowInventory(render, n);

        n++; // Space
        for (Monster* monstersinroom : this->mobs->GetMonsters(this->loc.CurrentRoom->Coords))
        {
            //render.Print( monstersinroom->Name + " ("+ monstersinroom->LastMessage +") Moves: " + std::to_string(monstersinroom->Moves), monstersinroom->Color, n);
            render.Print("* " + monstersinroom->GetCharacterString(), monstersinroom->Color, n, 0);
        }


        render.Update();
        this->prompt.InputPrompt(" >");


        if (!this->prompt.EvaluatePrompt())
        {
            // no match, what to do?
        }
        std::cout << "redraw" << std::endl;
    }
}
