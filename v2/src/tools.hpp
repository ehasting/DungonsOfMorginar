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
namespace DofM
{
    class Tools
    {
    public:
        Tools(Tools const &) = delete;

        void operator=(Tools const &) = delete;

        Tools();

    private:
        std::mt19937 rng;
    };
}

#endif //V2_TOOLS_H
