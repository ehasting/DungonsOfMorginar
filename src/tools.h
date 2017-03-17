/*
MIT License

Copyright (c) 2017 Egil Hasting

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef TOOLS_H
#define TOOLS_H
#include <string>
#include <locale>
#include <vector>
#include <random>
#include <iostream>

class Tools
{
public:
    static Tools& getInstance()
    {
        static Tools    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    Tools(Tools const&) = delete;
    void operator=(Tools const&) = delete;
    Tools();

    std::string toLowercase(std::string text)
    {
        std::string output("");
        std::locale loc;
        for(auto elem : text)
            output.push_back((char)std::tolower(elem, loc));
        return output;
    }
    const std::vector<std::string> explode(const std::string& s, const char& c)
    {
        std::string buff{""};
        std::vector<std::string> v;

        for(auto n:s)
        {
            if(n != c) buff+=n; else
                if(n == c && buff != "") { v.push_back(buff); buff = ""; }
        }
        if(buff != "") v.push_back(buff);

        return v;
    }
    int Dice(int min, int max)
    {
        std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
        int retvalue = dist6(this->rng);

        //std::cout << "min("<< min << ") " << "max("<< max << ") rnd: " << retvalue << std::endl;

        return retvalue;
    }
private:
    std::mt19937 rng;

};
#endif // TOOLS_H

