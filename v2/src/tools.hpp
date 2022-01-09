//
// Created by eh on 1/4/22.
//

#ifndef V2_TOOLS_H
#define V2_TOOLS_H

#include <string>
#include <locale>
#include <vector>
#include <random>
#include <iostream>
#include <mutex>
#include <chrono>
#include <typeinfo>
#include <memory>

#include <termios.h>
#include <stdlib.h>
#include <stdio.h>


namespace DofM
{
    class Tools
    {
    public:
        Tools(Tools const &) = delete;

        void operator=(Tools const &) = delete;

        Tools();
        ~Tools();


    private:

        std::mt19937 rng;
    };
}

#endif //V2_TOOLS_H
