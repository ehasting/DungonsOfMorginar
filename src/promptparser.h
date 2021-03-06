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
#ifndef PROMPTPARSER_H
#define PROMPTPARSER_H
#include <string>
#include "tools.h"
#include <vector>
#include "action.h"
#include <iostream>

class PromptParser
{
private:
    std::string input;

public:
    PromptParser();
    void InputPrompt(std::string);
    std::vector<Action> Actions;

    std::string GetCommands()
    {
        return this->input;
    }
    std::vector<std::string> GetCommandList()
    {
        return Tools::getInstance().explode(this->input, ' ');
    }

    bool IsCommand(std::string requestedcommand)
    {
        return (this->input.find(requestedcommand) != std::string::npos);
    }

    bool EvaluatePrompt()
    {
        for (auto cmd : this->Actions)
        {
            if (this->IsCommand(cmd.Keyword))
            {
                std::cout << "FOUND: " << cmd.Keyword << std::endl;
                cmd.ActionFunction();
                return true;
            }
        }
        return false;
    }

};

#endif // PROMPTPARSER_H
