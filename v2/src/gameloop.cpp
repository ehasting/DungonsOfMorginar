//
// Created by eh on 1/4/22.
//

#include "gameloop.hpp"
#include <chrono>

GameLoop::GameLoop()
{
    this->EventLoop = std::make_shared<std::thread>([this] {
        std::cout << "Starting EventLoop" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds (5) );
        this->IsRunning = false;
    } );

}

void GameLoop::Run()
{
    std::cout << "Starting GameLoop::Run" << std::endl;
    while(this->IsRunning)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20) );
    }
    this->EventLoop->join();
}