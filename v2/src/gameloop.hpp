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
#include "iohandlershared.hpp"

#if defined(WIN64)
#include "platform/windowsterminal.hpp"
#elif defined(LINUX)
#include "platform/linuxterminal.hpp"
#else
#error "Unable to determine OS or current OS is not supported!"
#endif

namespace DofM
{
    class GameLoop
    {
    private:
        std::vector<std::shared_ptr<DynamicObject> > DynamicObjects;
        std::shared_ptr<std::thread> MainEventThread;
        std::shared_ptr<std::thread> DrawThread;

        void MainEventWorker();
        void DrawLoopWorker();
        DofM::Tools ToolsObject;
        IOHandlerShared InHandler;
        std::shared_ptr<ITerminal> NativeTerminal;
        std::unique_ptr<NonBlockingTerminal> Term;
        std::stringstream TextCommandBuffer;

    public:
        GameLoop();
        ~GameLoop();
        void Run();
        bool IsRunning = true;
    };
}

#endif //DUNGONSOFMORGINAR_GAMELOOP_H
