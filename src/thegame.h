#ifndef THEGAME_H
#define THEGAME_H
#include "promptparser.h"
#include "locations.h"
#include "character.h"

class TheGame
{
private:
    PromptParser prompt;
    Locations loc;
    Character hero;
    bool running;
    AsciiRenderEngine render;
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
