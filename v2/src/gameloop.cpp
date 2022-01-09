//
// Created by eh on 1/4/22.
//

#include "gameloop.hpp"
#include <chrono>
#include <typeinfo>
#include "map/mapregions.hpp"




namespace DofM
{
    GameLoop::GameLoop()
    {
        Term.SetupNonBlockingTerminal();


        std::shared_ptr<Character> n = std::make_shared<Character>("Hero 2000", Location("character", 1, 1, 1));
        std::shared_ptr<Mouse> m = std::make_shared<Mouse>("Arnold");

        //auto l = n->GetDynamicObject();
        this->DynamicObjects.push_back(n);
        this->DynamicObjects.push_back(m);

        this->MainEventThread = std::make_shared<std::thread>([this]{ this->MainEventWorker(); });
        this->KeyboardEventThread = std::make_shared<std::thread>([this]{ this->CheckForKeyboardEventWorker(); });

    }

    GameLoop::~GameLoop()
    {
        //std::cout << "In buffer: " << this->TextCommandBuffer.str() << std::endl;
        this->DynamicObjects.clear();
        std::cout << this->Term.KeyLog <<std::endl;
    }



    void GameLoop::MainEventWorker()
    {
        //std::cout << "Starting MainEventWorker" << std::endl;
        while (this->IsRunning)
        {
            int rowoffset = 12;
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
                Term.WriteToBuffer(std::to_string(rowoffset) + "::" + n->GetDescriptionLine(), ScreenPos(6, rowoffset), Term.ColMax - 10);
                rowoffset++;
            }
            Term.FillScreen();

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        }
        //std::cout << "Stopping MainEventWorker" << std::endl;
    }


    void GameLoop::CheckForKeyboardEventWorker()
    {
        fd_set read_fds;
        int result;
        int sfd = 0;
        //std::cout << "Starting CheckForKeyboardEventWorker" << std::endl;
        char c = 0;
        uint iter = 1;
        while (this->IsRunning)
        {
            Term.ScanKeyboardInput();

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            Term.Redraw();
            iter++;
        }
      //  std::cout << "Stopping CheckForKeyboardEventWorker" << std::endl;
    }

    void GameLoop::Run()
    {
        //std::cout << "Starting GameLoop::Run" << std::endl;
        MapRegions reg;
        //reg.DrawRegion(MapRegions::Region1);



        // exit after 10 sec
        std::this_thread::sleep_for(std::chrono::seconds (5));
        this->IsRunning = false;

        this->KeyboardEventThread->join();
        this->MainEventThread->join();

    }
}