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

        std::shared_ptr<Character> n = std::make_shared<Character>(Location("home", 1, 2, 3));
        std::shared_ptr<Mouse> m = std::make_shared<Mouse>();

        //auto l = n->GetDynamicObject();
        this->DynamicObjects.push_back(n);
        this->DynamicObjects.push_back(m);
        this->EventLoop = std::make_shared<std::thread>([this]
                                                        {
                                                            std::cout << "Starting EventLoop" << std::endl;
                                                            std::this_thread::sleep_for(std::chrono::seconds(5));
                                                            this->IsRunning = false;
                                                        });

    }

    GameLoop::~GameLoop()
    {
        this->DynamicObjects.clear();
    }

    void GameLoop::Run()
    {
        std::cout << "Starting GameLoop::Run" << std::endl;
        MapRegions reg;
        reg.DrawRegion(MapRegions::Region1);

        while (this->IsRunning)
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
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        this->EventLoop->join();
    }
}