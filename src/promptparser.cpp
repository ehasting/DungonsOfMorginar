#include "promptparser.h"
#include "tools.h"
#include <string>
#include <iostream>

PromptParser::PromptParser()
{

}

void PromptParser::InputPrompt(std::string inputText)
{
    std::cout << inputText << " ";
    std::getline(std::cin, this->input);
    std::string lowercaseInput = Tools::toLowercase(this->input);
}


