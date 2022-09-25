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
#if defined(WIN64)
        NativeTerminal = std::make_shared<WindowsTerminal>();
        InHandler = std::make_shared<SharedInputHandler>();
#elif defined(LINUX)
        NativeTerminal = std::make_shared<LinuxTerminal>();
#endif
        this->NativeTerminal->SetupNonBlockingTerminal();
        this->Term = std::make_unique<NonBlockingTerminal>(this->NativeTerminal, this->InHandler);

        std::cout <<  "TERMINAL SETUP DONE" << std::endl;
        std::shared_ptr<Character> n = std::make_shared<Character>("Hero 2000", Location("character", 1, 1, 1));
        std::shared_ptr<Mouse> m = std::make_shared<Mouse>("Arnold");

        //auto l = n->GetDynamicObject();
        this->DynamicObjects.push_back(n);
        this->DynamicObjects.push_back(m);

        this->MainEventThread = std::make_shared<std::thread>([this]{ this->MainEventWorker(); });
        this->DrawThread = std::make_shared<std::thread>([this]{ this->DrawLoopWorker(); });

    }

    GameLoop::~GameLoop()
    {
        //std::cout << "In buffer: " << this->TextCommandBuffer.str() << std::endl;
        this->DynamicObjects.clear();
        std::cout << this->InHandler->KeyLog <<std::endl;
    }


    // Drawing related stuff needs to be here.
    void GameLoop::DrawLoopWorker()
    {
        while (this->IsRunning)
        {
            int rowoffset = 12;
            for (auto n: this->DynamicObjects)
            {
                if (n->GetTypeName() == Character::TypeName)
                {
                    //std::cout << n->GetRealObject<Character>()->Mastrubate() << std::endl;
                } else if (n->GetTypeName() == Mouse::TypeName)
                {
                    //std::cout << n->GetRealObject<Mouse>()->Moustrubate() << std::endl;
                }

                Term->WriteToBuffer(std::to_string(rowoffset) + "::" + n->GetDescriptionLine(),
                                   ScreenPos(6, rowoffset), Term->ColMax - 10);
                rowoffset++;
            }
            Term->WriteToBuffer("Test buffer: " + InHandler->KeyLog, ScreenPos(7, rowoffset+1), 32);
            Term->FillScreen();
            Term->Redraw();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void GameLoop::MainEventWorker()
    {
        auto lasttick = std::chrono::steady_clock::now();
        //std::cout << "Starting MainEventWorker" << std::endl;
        while (this->IsRunning)
        {
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lasttick).count() > 999)
            {
                // Update all DynamicObjects
                for (auto n: this->DynamicObjects)
                {
                    if (n->GetTypeName() == Character::TypeName)
                    {
                        //std::cout << n->GetRealObject<Character>()->Mastrubate() << std::endl;
                    } else if (n->GetTypeName() == Mouse::TypeName)
                    {
                        //std::cout << n->GetRealObject<Mouse>()->Moustrubate() << std::endl;
                    }
                    n->Update(0);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        //std::cout << "Stopping MainEventWorker" << std::endl;
    }




    void GameLoop::Run()
    {
        std::cout << "Starting GameLoop::Run" << std::endl;
        //MapRegions reg;
        //reg.DrawRegion(MapRegions::Region1);



        // exit after 10 sec

        std::this_thread::sleep_for(std::chrono::seconds(60));
        this->IsRunning = false;

        Term->Terminate();
        this->MainEventThread->join();
        this->DrawThread->join();

    }
}