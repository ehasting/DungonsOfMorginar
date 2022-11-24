enum TermRendringAttrs
{
    RESET = 0,
    BOLD = 1,
    BLINK = 5,
    REV_BLINK = 7,

    BG_BLACK = 40,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_BROWN = 43,
    BG_BLUE = 44,
    BG_MAGENTA = 45,
    BG_CYAN = 46,
    BG_WHITE = 47,
    BG_BLACK_BR = 100,
    BG_RED_BR = 101,
    BG_GREEN_BR = 102,
    BG_BROWN_BR = 103,
    BG_BLUE_BR = 104,
    BG_MAGENTA_BR = 105,
    BG_CYAN_BR = 106,
    BG_WHITE_BR = 107,
    FG_BLACK = 30,
    FG_RED = 31,
    FG_GREEN = 32,
    FG_BROWN = 33,
    FG_BLUE = 34,
    FG_MAGENTA = 35,
    FG_CYAN = 36,
    FG_WHITE = 37,
    FG_BLACK_BR = 90,
    FG_RED_BR = 91,
    FG_GREEN_BR = 92,
    FG_BROWN_BR = 93,
    FG_BLUE_BR = 94,
    FG_MAGENTA_BR = 95,
    FG_CYAN_BR = 96,
    FG_WHITE_BR = 97
}

enum KeyPress
{
    UP, DOWN, RIGHT, LEFT, //3
    HOME, END, //5
    PG_UP, PG_DOWN, //7
    INSERT, _DELETE, BACKSPACE, //10
    RETURN, //11
    F1, F2, F3, F4, F5, F6, F7, F8, //19
    ESC, //20
    ALPHA,
    NUMBER,
    SPECIAL_CHARACTER,
    UNDETECTED_ESCAPE_SEQUENCE = 9998,
    NO_KEY = 9999
}