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
#include "core/inputhandler.hpp"
#include "actionparser.hpp"
#include "map/mapregions.hpp"
#include "systems/namingsystem.hpp"
#ifdef __WIN64__
#include "platform/windowsterminal.hpp"
#elif __linux__
#include "platform/linuxterminal.hpp"
#elif __HAIKU__
#include "platform/linuxterminal.hpp"
#endif

namespace DofM
{
    class GameLoop
    {
    private:
        DynamicObject::DynamicObjectList DynamicObjects;
        std::shared_ptr<std::thread> MainEventThread;
        std::shared_ptr<std::thread> InputProcessThread;
        DofM::Tools ToolsObject;
        std::shared_ptr<ITerminal> NativeTerminal;
        std::shared_ptr<NonBlockingTerminal> Term;
        std::stringstream TextCommandBuffer;
        void MainEventWorker();
        void DrawLoopWorker();
        void InputProcessorWorker();
        ActionParser Parser;
        DofM::NamingSystem Names;
        long MainEventFrameTimeNS = 0;


    public:
        GameLoop();
        ~GameLoop();
        void Run();
        void ProcessKeyPressEventCallback(std::vector<SDL_Scancode> scancode);
        void ProcessKeyTextCallback(std::string text);
        bool IsRunning = true;
        std::string KeyLog;
        void Terminate();
        long long int TickCounter = 0;
        std::shared_ptr<Character> Hero;
        MapRegions::SMapRegions TestMap;

    };
}

#endif //DUNGONSOFMORGINAR_GAMELOOP_H
