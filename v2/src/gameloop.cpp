//
// Created by eh on 1/4/22.
//

#include "gameloop.hpp"
#include <chrono>
#include <typeinfo>
#include "map/mapregions.hpp"
#include <memory>


namespace DofM
{
GameLoop::GameLoop()
{
    this->DynamicObjects = std::make_shared<std::vector<std::shared_ptr<DynamicObject>>>();
    NativeTerminal = std::make_shared<LinuxTerminal>();
    this->NativeTerminal->SetupNonBlockingTerminal();
    this->Term = std::make_shared<NonBlockingTerminal>(this->NativeTerminal);

    this->TestMap = std::make_shared<MapRegions>(
        "Prison Cell 352 øæå █▄▌▐▀█",
        std::make_shared<Location>(0,0,0),
        std::make_shared<Location>(16, 9, 0));
    this->TestMap->Description = "You are in a prison cell, there is a table in the middle of the room.";
    this->TestMap->AddWallsToRoom();
    MapObject n0;
    n0.StartLocation = std::make_shared<Location>(5,5,0);
    n0.StopLocation = std::make_shared<Location>(7,6,0);
    this->TestMap->MapObjects.push_back(n0);



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

    this->MainEventThread = std::make_shared<std::thread>([this]{ this->MainEventWorker(); });
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


    while (this->IsRunning)
    {

        int rowoffset = 25;
        int lineseparator = 24;
        auto innermapoffset = ScreenPos(5,5);
        auto outermapoffset = ScreenPos(4,4);
        next_tick = SDL_GetTicks() + (1000 / 20);

        this->TestMap->DrawMap(Term, outermapoffset);

        Term->WriteToBuffer( ToolsObject.RepeateString("█",Term->ColMax) , ScreenPos(0,lineseparator));
        for(int x = 0; x < lineseparator; x++)
        {
            Term->WriteToBuffer("│", ScreenPos((Term->ColMax/3)*2, x));
        }
        for (auto n: *this->DynamicObjects)
        {
            if (n->GetTypeName() == Mouse::TypeName)
            {
                auto cc = n->GetRealObject<Mouse>();
                auto pos = cc->ObjectLocation->ReturnAsScreenPos().AddOffset(outermapoffset);
                /*
                auto pos = ScreenPos(
                    cc->ObjectLocation->X - this->TestMap->StartLocation->X,
                    cc->ObjectLocation->Y - this->TestMap->StartLocation->Y
                    );
                */
                Uint8 red = (0 + cc->UniqueNameHash % 255);
                Uint8 green = (127 + cc->UniqueNameHash % 127);
                Uint8 blue = (127 + cc->UniqueNameHash % 127);
                Uint8 alpha = 255;
                SDL_Color cColor = {red, green, blue, alpha};
                if (cc->IsAlive())
                {
                    Term->WriteToBuffer("☺", pos, cColor);
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
                Term->WriteToBuffer("0", pos, cColor, 1);
            }

            Term->WriteToBuffer(n->GetDescriptionLine(), ScreenPos(2, rowoffset));
            rowoffset++;
        }
        Term->WriteToBuffer(fmt::format("Test buffer: {}", this->KeyLog), ScreenPos(7, rowoffset+1));
        Term->WriteToBuffer(fmt::format("Tick delivered on {}ns", this->MainEventFrameTimeNS), ScreenPos(60, 0), {0,255,0,128});
        {
            Term->Redraw();
        }
        sleep_ticks = next_tick - SDL_GetTicks();
        if (sleep_ticks > 0)
        {
            SDL_Delay(sleep_ticks);
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void GameLoop::MainEventWorker()
{
    long ticklengthms = 100;
    auto lasttick = std::chrono::steady_clock::now();
    //std::cout << "Starting MainEventWorker" << std::endl;
    while (this->IsRunning)
    {
        auto now = std::chrono::steady_clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - lasttick).count();
        if (dur >= ticklengthms)
        {
            this->TickCounter++;
            // Update all DynamicObjects
            for (auto n: *this->DynamicObjects)
            {
                if (n->GetTypeName() == Character::TypeName)
                {
                    //std::cout << n->GetRealObject<Character>()->Mastrubate() << std::endl;
                }
                else if (n->GetTypeName() == Mouse::TypeName)
                {
                    //std::cout << n->GetRealObject<Mouse>()->Moustrubate() << std::endl;
                }
                n->Update(TickCounter);
            }
            // Update tick
            lasttick = std::chrono::steady_clock::now();
            this->MainEventFrameTimeNS = std::chrono::duration_cast<std::chrono::nanoseconds>(lasttick - now).count();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    //std::cout << "Stopping MainEventWorker" << std::endl;
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
