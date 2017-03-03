#include "asciirenderengine.h"
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

AsciiRenderEngine::AsciiRenderEngine()
{
    this->RESETCOLOR = "\033[0m";
#ifdef _WIN32
    std::cout << "Require win implementation.. exiting";
    exit(1);
#endif
    this->Cursor = 0;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    this->TermHeight = w.ws_row-1;
    this->TermWidth = w.ws_col;
    std::cout.setf(std::ios::unitbuf);
    //this->ScreenBuffer.reserve(this->TermHeight);
    for (int cnt = 0; cnt < this->TermHeight; cnt++)
    {
        this->ScreenBuffer.push_back(std::string(this->TermWidth-2, ' '));
    }
}

bool AsciiRenderEngine::IsTerminalSizeChanged()
{
    struct winsize termw;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &termw);
    int h = termw.ws_row-1;
    int w = termw.ws_col;
    if ( h != this->TermHeight )
        return true;
    if ( w != this->TermWidth )
        return true;
    return false;
}

void AsciiRenderEngine::Print(std::string text)
{
    this->ScreenBuffer[this->Cursor] = text;
    this->Cursor++;
}

std::string AsciiRenderEngine::ResolveColor(COLOR color)
{
    switch(color)
    {
        case AsciiRenderEngine::RED:
            return "\033[0;31m";
        case AsciiRenderEngine::BLUE:
            return "\033[0;34m";
        case AsciiRenderEngine::GREEN:
            return "\033[0;32m";
        case AsciiRenderEngine::WHITE:
            return "\033[0;37m";
        case AsciiRenderEngine::YELLOW:
            return "\033[0;33m";

    }
}

void AsciiRenderEngine::Print(std::string text, COLOR color, int line, int lineoffset)
{
    std::string output;
    int currline = line;
    if (text.length() > this->TermWidth)
    {
        int cnt = 0;
        for (auto ch : text)
        {
            if (((char)ch == ' ') && cnt > (this->TermWidth/4)*3 )
            {
                output.push_back((char)ch);
                this->ScreenBuffer[currline++] = this->ResolveColor(color) + std::string(lineoffset, ' ') + output + this->RESETCOLOR;
                output.clear();
                cnt = 0;
            }
            else
                output.push_back((char)ch);
            cnt++;
        }
        this->ScreenBuffer[currline++] = this->ResolveColor(color) + std::string(lineoffset, ' ') + output + this->RESETCOLOR;
    }
    else {
        this->ScreenBuffer[currline] = this->ResolveColor(color) + std::string(lineoffset, ' ') + text + this->RESETCOLOR;
    }
}


void AsciiRenderEngine::Update()
{
    int cnt = 0;
    for(auto line : this->ScreenBuffer)
    {
        std::cout << line << std::endl;
        this->ScreenBuffer[cnt] = std::string(this->GetLastChar()-1, ' ');
        cnt++;
    }
}

void AsciiRenderEngine::PrintDebug()
{
    std::cout << "Lines: " << this->TermHeight << std::endl;
    std::cout << "Columns: " << this->TermWidth << std::endl;
}


void AsciiRenderEngine::ClearScreen()
{
    std::cout << std::string( this->TermHeight, '\n' );
}
