#ifndef TOOLS_H
#define TOOLS_H
#include <string>
#include <character.h>
#include <location.h>
#include <locale>
#include <vector>

class Tools
{
public:
    Tools();
    static std::string toLowercase(std::string text)
    {
        std::string output("");
        std::locale loc;
        for(auto elem : text)
            output.push_back((char)std::tolower(elem, loc));
        return output;
    }
    static const std::vector<std::string> explode(const std::string& s, const char& c)
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

};

#endif // TOOLS_H
