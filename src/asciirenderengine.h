#ifndef ASCIIRENDERENGINE_H
#define ASCIIRENDERENGINE_H
#include <iostream>
#include <vector>
#include "renderengine.h"

class AsciiRenderEngine : RenderEngine
{
private:
    int TermHeight;
    int TermWidth;
    int Cursor;
public:
    std::string RESETCOLOR;
    enum COLOR { RED, BLUE, WHITE, GREEN, YELLOW };
    int GetLastLine()
    {
        return this->TermHeight-1;
    }
    int GetLastChar()
    {
        return this->TermWidth-1;
    }
    bool IsTerminalSizeChanged();
    AsciiRenderEngine();
    void ClearScreen();
    void PrintDebug();
    std::vector<std::string> ScreenBuffer;
    void Print(std::string text);
    void Print(std::string text, COLOR color, int line, int lineoffset = 0);
    void Update();
    std::string ResolveColor(COLOR color);
};

#endif // ASCIIRENDERENGINE_H
