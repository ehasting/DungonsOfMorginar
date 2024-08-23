//
// Created by eh on 1/4/22.
//
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>
#include "tools.hpp"
#include "gameloop.hpp"
#include <fstream>



auto run() -> void
{
    std::cout << "Test from run()" << std::endl;
}

int main (int argc, char *argv[])
{

    //auto n = std::make_shared<DofM::LinuxTerminal>();
    Uint32 next_tick = 0;
    int sleep_ticks = 0;
    bool terminate = false;


    DofM::Tools bungo;
    DofM::GameLoop loop;
    loop.Run();

}
