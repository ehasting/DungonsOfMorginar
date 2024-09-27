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
                "Prison Cell 352",
                std::make_shared<Location>(0,0,0),
                std::make_shared<Location>(7, 7, 0));

        this->Hero = std::make_shared<Character>("Hero 2000", std::make_shared<Location>(1, 1, 1), this->DynamicObjects);
        this->Hero->SetMap(this->TestMap);
        auto m1 = std::make_shared<Mouse>("Arnold", std::make_shared<Location>(4, 4, 1), this->DynamicObjects);
        m1->SetMap(this->TestMap);
        auto m2 = std::make_shared<Mouse>("Sylvester", std::make_shared<Location>(1, 1, 1), this->DynamicObjects);
        m2->SetMap(this->TestMap);
        auto m3 = std::make_shared<Mouse>("MacClain", std::make_shared<Location>(4, 4, 1), this->DynamicObjects);
        m3->SetMap(this->TestMap);
        auto m4 = std::make_shared<Mouse>("Wick", std::make_shared<Location>(1, 1, 1), this->DynamicObjects);
        m4->SetMap(this->TestMap);
        auto m5 = std::make_shared<Mouse>("Connor", std::make_shared<Location>(4, 4, 1), this->DynamicObjects);
        m5->SetMap(this->TestMap);
        auto m6 = std::make_shared<Mouse>("Rambo", std::make_shared<Location>(1, 1, 1), this->DynamicObjects);
        m6->SetMap(this->TestMap);
        auto m7 = std::make_shared<Mouse>("Han Solo", std::make_shared<Location>(1, 1, 1), this->DynamicObjects);
        m7->SetMap(this->TestMap);
        //auto l = n->GetDynamicObject();
        this->DynamicObjects->push_back(this->Hero);
        this->DynamicObjects->push_back(m1);
        this->DynamicObjects->push_back(m2);
        this->DynamicObjects->push_back(m3);
        this->DynamicObjects->push_back(m4);
        this->DynamicObjects->push_back(m5);
        this->DynamicObjects->push_back(m6);
        this->DynamicObjects->push_back(m7);

        this->MainEventThread = std::make_shared<std::thread>([this]{ this->MainEventWorker(); });
        this->InputProcessThread = std::make_shared<std::thread>([this] { this->InputProcessorWorker(); });

    }

    GameLoop::~GameLoop()
    {
        //std::cout << "In buffer: " << this->TextCommandBuffer.str() << std::endl;
        this->DynamicObjects->clear();
        std::cout << this->KeyLog <<std::endl;
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
                    this->Term->WriteToBuffer(fmt::format("[BUFFER]: UP"), ScreenPos(4, 5), 16);
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
                    this->Term->WriteToBuffer(fmt::format("[BUFFER]: ESC"), ScreenPos(4, 5), 16);
                    this->Terminate();
                    break;
                case SDL_SCANCODE_F5:
                    this->Term->WriteToBuffer(fmt::format("[BUFFER]: F5"), ScreenPos(4, 5), 16);
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
                    this->Term->WriteToBuffer(fmt::format("[Response]: {}", response), ScreenPos(4, 5), 16);
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
            int rowoffset = 18;
            next_tick = SDL_GetTicks() + (1000 / 20);

            Term->WriteToBuffer(fmt::format("Room: {}", this->TestMap->UniqueName), ScreenPos(0,0), Term->RowMax, {255,0,64,255});
            Term->WriteToBuffer(fmt::format("{}", std::string(this->TestMap->Width(), '-')), ScreenPos(0,1), this->TestMap->Width());
            for(int x = 0; x < this->TestMap->Height(); x++)
            {
                Term->WriteToBuffer(fmt::format("{}", std::string(this->TestMap->Width(), '-')), ScreenPos(0,x+2), this->TestMap->Width());
            }
            Term->WriteToBuffer(fmt::format("{}", std::string(this->TestMap->Width(), '-')), ScreenPos(0,this->TestMap->Height()+2), this->TestMap->Width());

            Term->WriteToBuffer(std::string(Term->ColMax,'_'), ScreenPos(0,16), Term->ColMax);
            for (auto n: *this->DynamicObjects)
            {
                if (n->GetTypeName() == Mouse::TypeName)
                {
                    auto cc = n->GetRealObject<Mouse>();
                    auto pos = ScreenPos(
                            cc->ObjectLocation->X - this->TestMap->StartLocation->X,
                            cc->ObjectLocation->Y - this->TestMap->StartLocation->Y
                    );
                    if (cc->IsAlive())
                    {
                        Term->WriteToBuffer("X", pos, 1, {255, 0, 128, 255});
                    }
                    else
                    {
                        Term->WriteToBuffer("*", pos, 1, {255, 128, 0, 255});
                    }
                }
                else if (n->GetTypeName() == Character::TypeName)
                {
                    auto cc = n->GetRealObject<Character>();
                    //Term->WriteToBuffer("0", cc->ObjectLocation->OffsetLocation(this->TestMap->StartLocation)->ReturnAsScreenPos(), 1);
                }

                Term->WriteToBuffer(n->GetDescriptionLine(),
                                   ScreenPos(2, rowoffset), Term->ColMax);
                rowoffset++;
            }
            Term->WriteToBuffer("Test buffer: " + this->KeyLog, ScreenPos(7, rowoffset+1), 32);
            Term->Redraw();
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
            auto currentdurationms = std::chrono::duration_cast<std::chrono::milliseconds>(now - lasttick).count();
            if (currentdurationms >= ticklengthms)
            {
                this->TickCounter++;
                Term->WriteToBuffer(fmt::format("Tick delivered on {}ms", currentdurationms), ScreenPos(40, 0), 30);
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
