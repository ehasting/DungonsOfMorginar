//
// Created by eh on 1/4/22.
//

#ifndef DUNGONSOFMORGINAR_GAMELOOP_H
#define DUNGONSOFMORGINAR_GAMELOOP_H
#include <thread>
#include <memory>
#include <iostream>
#include <vector>
#include <stdexcept>

#include "models/dynamicobject.hpp"
#include "models/character.hpp"
#include "models/mouse.hpp"

class GameLoop
{
private:
    std::vector<std::shared_ptr<DynamicObject> > DynamicObjects;
    std::shared_ptr<std::thread> EventLoop;

public:
//std::thread thObj(<CALLBACK>);
    GameLoop();
    ~GameLoop();
    void Run();
    bool IsRunning = true;
};


#endif //DUNGONSOFMORGINAR_GAMELOOP_H
