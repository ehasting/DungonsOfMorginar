//
// Created by eh on 1/4/22.
//

#include "gameloop.hpp"
#include <chrono>
#include <typeinfo>
#include "map/mapregions.hpp"
#include "map/tile.hpp"
#include <memory>


namespace DofM
{
GameLoop::GameLoop()
{
    this->DynamicObjects = std::make_shared<std::vector<std::shared_ptr<DynamicObject>>>();
    NativeTerminal = std::make_shared<LinuxTerminal>();
    this->NativeTerminal->SetupNonBlockingTerminal();
    this->Term = std::make_shared<NonBlockingTerminal>(this->NativeTerminal);

    std::vector<Location::SLocation> testmapexits;
    testmapexits.push_back(std::make_shared<Location>(0, 3, 0));
    //testmapexits.push_back(std::make_shared<Location>(24, 3, 0));
    testmapexits.push_back(std::make_shared<Location>(12, 15, 0));
    this->TestMap = std::make_shared<MapRegions>(
        "Prison Cell 352 øæå █▄▌▐▀█",
        testmapexits
    );

    this->TestMap->Description = "You are in a prison cell, there is a table in the middle of the room.";
    this->TestMap->AddWallsToRoom();




    this->Hero = std::make_shared<Character>("Hero 2000", std::make_shared<Location>(1, 1, 1), this->DynamicObjects);
    this->Hero->SetMap(this->TestMap);
    auto m1 = std::make_shared<Mouse>(this->Names.GetRandomName(), std::make_shared<Location>(4, 4, 1), this->DynamicObjects);
    m1->SetMap(this->TestMap);
    auto m2 = std::make_shared<Mouse>(this->Names.GetRandomName(), std::make_shared<Location>(1, 1, 1), this->DynamicObjects);
    m2->SetMap(this->TestMap);
    auto m3 = std::make_shared<Mouse>(this->Names.GetRandomName(), std::make_shared<Location>(4, 4, 1), this->DynamicObjects);
    m3->SetMap(this->TestMap);
    auto m4 = std::make_shared<Mouse>(this->Names.GetRandomName(), std::make_shared<Location>(1, 1, 1), this->DynamicObjects);
    m4->SetMap(this->TestMap);
    auto m5 = std::make_shared<Mouse>(this->Names.GetRandomName(), std::make_shared<Location>(4, 4, 1), this->DynamicObjects);
    m5->SetMap(this->TestMap);
    auto m6 = std::make_shared<Mouse>(this->Names.GetRandomName(), std::make_shared<Location>(1, 1, 1), this->DynamicObjects);
    m6->SetMap(this->TestMap);
    auto m7 = std::make_shared<Mouse>(this->Names.GetRandomName(), std::make_shared<Location>(1, 1, 1), this->DynamicObjects);
    m7->SetMap(this->TestMap);
    auto m8 = std::make_shared<Mouse>(this->Names.GetRandomName(), std::make_shared<Location>(1, 1, 1), this->DynamicObjects);
    m8->SetMap(this->TestMap);
    auto m9 = std::make_shared<Mouse>(this->Names.GetRandomName(), std::make_shared<Location>(1, 1, 1), this->DynamicObjects);
    m9->SetMap(this->TestMap);
    auto m10 = std::make_shared<Mouse>(this->Names.GetRandomName(), std::make_shared<Location>(1, 1, 1), this->DynamicObjects);
    m10->SetMap(this->TestMap);
    auto m11 = std::make_shared<Mouse>(this->Names.GetRandomName(), std::make_shared<Location>(1, 1, 1), this->DynamicObjects);
    m11->SetMap(this->TestMap);
    auto m12 = std::make_shared<Mouse>(this->Names.GetRandomName(), std::make_shared<Location>(1, 1, 1), this->DynamicObjects);
    m12->SetMap(this->TestMap);
    //auto l = n->GetDynamicObject();
    this->DynamicObjects->push_back(this->Hero);
    this->DynamicObjects->push_back(m1);

    this->DynamicObjects->push_back(m2);
    this->DynamicObjects->push_back(m3);
    this->DynamicObjects->push_back(m4);
    this->DynamicObjects->push_back(m5);
    this->DynamicObjects->push_back(m6);
    this->DynamicObjects->push_back(m7);
    this->DynamicObjects->push_back(m8);
    this->DynamicObjects->push_back(m9);
    this->DynamicObjects->push_back(m10);
    this->DynamicObjects->push_back(m11);
    this->DynamicObjects->push_back(m12);

    //this->MainEventThread = std::make_shared<std::thread>([this]{ this->MainEventWorker(); });
    this->InputProcessThread = std::make_shared<std::thread>([this] { this->InputProcessorWorker(); });

}

GameLoop::~GameLoop()
{
    //std::cout << "In buffer: " << this->TextCommandBuffer.str() << std::endl;
    this->DynamicObjects->clear();
    // std::cout << this->KeyLog <<std::endl;
    if (this->MainEventThread->joinable())
        this->MainEventThread->join();
    if (this->InputProcessThread->joinable())
        this->InputProcessThread->join();
}
void GameLoop::ProcessKeyTextCallback(std::string text)
{
    KeyLog += text;
}
void GameLoop::ProcessKeyPressEventCallback(std::vector<SDL_Scancode> scancode)
{
    for (auto &sc:scancode)
    {
        switch (sc)
        {
        case SDL_SCANCODE_UP:
            this->Term->WriteToBuffer(fmt::format("[BUFFER]: UP"), ScreenPos(4, 5));
            this->Hero->TryMoveNorth();
            break;
        case SDL_SCANCODE_DOWN:
            //this->Term->WriteToBuffer(fmt::format("[BUFFER]: DOWN"), ScreenPos(4, 5), 16);
            this->Hero->TryMoveSouth();
            break;
        case SDL_SCANCODE_LEFT:
            //this->Term->WriteToBuffer(fmt::format("[BUFFER]: UP"), ScreenPos(4, 5), 16);
            this->Hero->TryMoveWest();
            break;
        case SDL_SCANCODE_RIGHT:
            //this->Term->WriteToBuffer(fmt::format("[BUFFER]: DOWN"), ScreenPos(4, 5), 16);
            this->Hero->TryMoveEast();
            break;
        case SDL_SCANCODE_ESCAPE:
            this->Term->WriteToBuffer(fmt::format("[BUFFER]: ESC"), ScreenPos(4, 5));
            this->Terminate();
            break;
        case SDL_SCANCODE_F5:
            this->Term->WriteToBuffer(fmt::format("[BUFFER]: F5"), ScreenPos(4, 5));
            this->Terminate();
            break;
        case SDL_SCANCODE_BACKSPACE:
            if (!KeyLog.empty())
                KeyLog.pop_back();
            break;
        case SDL_SCANCODE_DELETE:
            KeyLog.clear();
            break;
        case SDL_SCANCODE_RETURN:
        {
            auto response = Parser.Parse(KeyLog);
            this->Term->WriteToBuffer(fmt::format("[Response]: {}", response), ScreenPos(4, 5));
            KeyLog.clear();
        }
        break;
        default:
            //this->DebugCallback(fmt::format("[BUFFER]: {}", detectedkeypress),
            //                   ScreenPos(4, 5), 16, emptyattr);
            break;
        }
    }
}

void GameLoop::InputProcessorWorker()
{
    while (this->IsRunning)
    {
        this->NativeTerminal->ScanKeyboardInput();
        this->ProcessKeyPressEventCallback(this->NativeTerminal->GetPendingEvents());
        this->ProcessKeyTextCallback(this->NativeTerminal->GetPendingText());
        if (this->NativeTerminal->IsTerminated)
        {
            this->Terminate();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
// Drawing related stuff needs to be here.
void GameLoop::DrawLoopWorker()
{
    Uint32 next_tick = 0;
    int sleep_ticks = 0;
    int ticks_to_death = 100;
    std::cout << "Starting DrawLoopWorker" << std::endl;
    while (this->IsRunning)
    {
        //ticks_to_death--;
        //if (ticks_to_death == 0)
        //    this->IsRunning = false;

        int rowoffset = 25;
        int lineseparator = 24;
        auto innermapoffset = ScreenPos(5,5);
        auto outermapoffset = ScreenPos(4,4);
        next_tick = SDL_GetTicks() + (1000 / 2);

        this->TestMap->DrawMap(Term, outermapoffset);

        Term->WriteToBuffer( ToolsObject.RepeateString("█",Term->ColMax) , ScreenPos(0,lineseparator));
        for(int x = 0; x < lineseparator; x++)
        {
            Term->WriteToBuffer("│", ScreenPos((Term->ColMax/3)*2, x));
        }
        for (auto &n: *this->DynamicObjects)
        {
            SDL_Color cColor = White;
            if (n->GetTypeName() == Mouse::TypeName)
            {
                auto cc = n->GetRealObject<Mouse>();
                auto pos = cc->ObjectLocation->ReturnAsScreenPos().AddOffset(outermapoffset);
                Uint8 red = (0 + cc->UniqueNameHash % 255);
                Uint8 green = (127 + cc->UniqueNameHash % 127);
                Uint8 blue = (127 + cc->UniqueNameHash % 127);
                Uint8 alpha = 255;
                cColor = {red, green, blue, alpha};
                if (cc->IsAlive())
                {
                    //Term->WriteToBuffer("☺", pos, cColor);
                }
                else
                {
                    Term->WriteToBuffer("*", pos, cColor);
                }
            }
            else if (n->GetTypeName() == Character::TypeName)
            {
                auto cc = n->GetRealObject<Character>();
                SDL_Color cColor = {255, 25, 64, 255};
                auto pos = cc->ObjectLocation->ReturnAsScreenPos().AddOffset(outermapoffset);
                //fmt::print("{}\n", pos.ToString());
                Term->WriteToBuffer("0", pos, cColor, 1);

            }

            Term->WriteToBuffer(n->GetDescriptionLine(), ScreenPos(2, rowoffset), cColor);
            rowoffset++;
        }
        Term->WriteToBuffer(fmt::format("Test buffer: {}", this->KeyLog), ScreenPos(7, rowoffset+1));
        Term->WriteToBuffer(fmt::format("Tick delivered on {}ns", this->MainEventFrameTimeNS), ScreenPos(60, 0), {0,255,0,128});
        {
            std::lock_guard<std::mutex> guard(this->Term->GetLock());
            Term->Redraw();
        }
        sleep_ticks = next_tick - SDL_GetTicks();
        if (sleep_ticks > 0)
        {
            SDL_Delay(sleep_ticks);
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << "Stopping DrawLoopWorker" << std::endl;
}

void GameLoop::MainEventInner()
{
    long ticklengthms = 100;
    auto lasttick = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - lasttick).count();
    if (dur >= ticklengthms)
    {
        std::lock_guard<std::mutex> guard(this->Term->GetLock());
        this->TickCounter++;
        // Update all DynamicObjects
        for (auto n: *this->DynamicObjects)
        {
            if (n->GetTypeName() == Character::TypeName)
            {
                n->Update(TickCounter);
                //n->GetRealObject<Character>()->Update(TickCounter);
            }
            else if (n->GetTypeName() == Mouse::TypeName)
            {
                n->Update(TickCounter);
                //std::cout << n->GetRealObject<Mouse>()->Moustrubate() << std::endl;
            }

        }
        // Update tick
        lasttick = std::chrono::steady_clock::now();
        this->MainEventFrameTimeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(lasttick - now).count();
    }

}

void GameLoop::MainEventWorker()
{
    std::cout << "Starting MainEventWorker" << std::endl;
    while (this->IsRunning)
    {
        this->MainEventInner();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "Stopping MainEventWorker" << std::endl;
}


void GameLoop::Terminate()
{
    this->IsRunning = false;

    Term->Terminate();

}

void GameLoop::Run()
{
    std::cout << "Starting GameLoop::Run" << std::endl;
    //MapRegions reg;
    //reg.DrawRegion(MapRegions::Region1);



    // exit after 10 sec

    //std::this_thread::sleep_for(std::chrono::seconds(60));
    this->DrawLoopWorker();
    //while (this->IsRunning)
    //{
    //    std::this_thread::sleep_for(std::chrono::seconds(1));
    //}
    //this->Terminate();

}
}
