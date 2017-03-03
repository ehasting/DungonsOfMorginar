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
        return Tools::explode(this->input, ' ');
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
