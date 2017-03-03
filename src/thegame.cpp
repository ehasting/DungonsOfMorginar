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


TheGame::TheGame()
{
    this->running = true;
}


void TheGame::SetupCommands()
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
                std::string deleteKey;
                for (auto ex : this->loc.CurrentRoom->Blockers)
                {
                    if (ex.second->Name == this->prompt.GetCommandList().at(1))
                    {
                        deleteKey = ex.first;
                        std::cout << "You used " << this->prompt.GetCommandList().at(1) << std::endl;
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
         this->loc.FindNewRoom(prompt.GetCommandList().at(1));
    };
    this->prompt.Actions.push_back(actionGo);
}

void TheGame::RecalculateScreen()
{
    if (this->render.IsTerminalSizeChanged())
    {
        this->render = AsciiRenderEngine();
        this->PLACEMENT_FEEDBACKLINE = render.GetLastLine();
        this->PLACEMENT_STATUSBAR = 0;
        this->PLACEMENT_STATUSBARSEP = this->PLACEMENT_STATUSBAR + 1;
        this->PLACEMENT_ROOMDISPLAY = int(render.GetLastLine() / 4) * 1;
        this->PLACEMENT_INVENTORY = int(render.GetLastLine()/4) * 3;
    }
}

void TheGame::Run()
{

    render.ClearScreen();

    //std::cout << "The Game" << std::endl;
    render.Print("The Game", AsciiRenderEngine::BLUE, 0);
    render.Print(std::string(render.GetLastChar()-1, '-'), AsciiRenderEngine::WHITE, 1);

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

    this->loc.Init();

    this->SetupCommands();

    //this->hero.ShowCharacter(this->render, 15);



    while(this->running)
    {
        this->RecalculateScreen();
        this->hero.ShowCharacterLine(render, this->PLACEMENT_STATUSBAR);
        render.Print(std::string(render.GetLastChar()-1, '-'), AsciiRenderEngine::WHITE, this->PLACEMENT_STATUSBARSEP);
        this->loc.CurrentRoom->ShowRoom(render, this->PLACEMENT_ROOMDISPLAY);
        this->hero.ShowInventory(render, this->PLACEMENT_INVENTORY);
        render.Update();
        this->prompt.InputPrompt(" >");

        render.Print(" -- you said: " +  prompt.GetCommands(), AsciiRenderEngine::GREEN, this->PLACEMENT_LASTCOMMAND);
        if (!this->prompt.EvaluatePrompt())
        {
            // default action if no command match!
            this->loc.FindNewRoom(prompt.GetCommands());
        }
    }
}
