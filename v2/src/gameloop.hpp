//
// Created by eh on 1/4/22.
//

#ifndef DUNGONSOFMORGINAR_GAMELOOP_H
#define DUNGONSOFMORGINAR_GAMELOOP_H
#include <thread>
#include <memory>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <future>
#include <string_view>
#include "fmt/core.h"
#include <mutex>

#include "tools.hpp"
#include "core/nonblockingterminal.hpp"
#include "coremodels/dynamicobject.hpp"
#include "objects/character.hpp"
#include "objects/mouse.hpp"
#include "core/keycodes.hpp"
#include "iohandler.hpp"

namespace DofM
{

    class GameLoop
    {
    private:
        std::vector<std::shared_ptr<DynamicObject> > DynamicObjects;
        std::shared_ptr<std::thread> MainEventThread;
        std::shared_ptr<std::thread> KeyboardEventThread;


        void MainEventWorker();
        void CheckForKeyboardEventWorker();
        DofM::Tools ToolsObject;
        IOHandler Term;
        std::stringstream TextCommandBuffer;

    public:
//std::thread thObj(<CALLBACK>);
        GameLoop();

        ~GameLoop();

        void Run();

        bool IsRunning = true;
    };
}

#endif //DUNGONSOFMORGINAR_GAMELOOP_H
