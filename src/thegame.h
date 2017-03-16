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
#ifndef THEGAME_H
#define THEGAME_H
#include "promptparser.h"
#include "locations.h"
#include "mazegenerator.h"
#include "hero.h"

class TheGame
{
private:
    PromptParser prompt;
    Locations loc;
    Hero hero;
    bool running;
    AsciiRenderEngine render;
    MazeGenerator maze;
public:
    TheGame();
    void SetupCommands();
    void Run();

    int PLACEMENT_ROOMDISPLAY = 3;
    int PLACEMENT_STATUSBAR = 0;
    int PLACEMENT_STATUSBARSEP = 1;
    int PLACEMENT_LASTCOMMAND = 2;
    int PLACEMENT_INVENTORY = 14;
    int PLACEMENT_FEEDBACKLINE;

    void RecalculateScreen();

};

#endif // THEGAME_H
