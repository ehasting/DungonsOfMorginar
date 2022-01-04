//
// Created by eh on 1/4/22.
//

#ifndef DUNGONSOFMORGINAR_GAMELOOP_H
#define DUNGONSOFMORGINAR_GAMELOOP_H
#include <thread>
#include <memory>
#include <iostream>

class GameLoop
{
private:
    std::shared_ptr<std::thread> EventLoop;
public:
//std::thread thObj(<CALLBACK>);
    GameLoop();
    void Run();
    bool IsRunning = true;
};


#endif //DUNGONSOFMORGINAR_GAMELOOP_H
