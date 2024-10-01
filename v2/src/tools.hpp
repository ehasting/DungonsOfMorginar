//
// Created by eh on 1/4/22.
//

#ifndef V2_TOOLS_H
#define V2_TOOLS_H

#if defined(_WIN64)

#elif defined(__linux__)
#include <termios.h>
#endif

#include <string>
#include <locale>
#include <vector>
#include <random>
#include <iostream>
#include <mutex>
#include <chrono>
#include <typeinfo>
#include <memory>
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
        int Dice(int max = 6, int min = 1)
        {
            std::uniform_int_distribution<> dis(min, max);
            return dis(this->rng);

        }

        std::string RepeateString(std::string ch, int length)
        {
            std::string n;
            for (int i = 0; i < length; ++i) {
                n.append(ch);
            }
            return n;
        }

        bool RndBool()
        {
            auto r = this->Dice(1, 0);
            if (r==0)
                return false;
            else
                return true;
        }
        int RndRange(int min, int max)
        {
            return this->Dice(max, min);
        }

        // trim from start (in place)
        inline void ltrim(std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                        return !std::isspace(ch);
                    }));
        }

        // trim from end (in place)
        inline void rtrim(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
                        return !std::isspace(ch);
                    }).base(), s.end());
        }
    private:

        std::mt19937 rng;
    };
}

#endif //V2_TOOLS_H
