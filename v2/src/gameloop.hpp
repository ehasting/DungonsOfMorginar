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

#include "coremodels/dynamicobject.hpp"
#include "objects/character.hpp"
#include "objects/mouse.hpp"
namespace DofM
{
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
}

#endif //DUNGONSOFMORGINAR_GAMELOOP_H
