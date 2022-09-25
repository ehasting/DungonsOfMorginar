//
// Created by EgilHasting on 25/09/2022.
//

#ifndef DUNGONSOFMORGINAR_WINDOWSTERMINAL_HPP
#define DUNGONSOFMORGINAR_WINDOWSTERMINAL_HPP
#include <Windows.h>
#include <WinCon.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <stdexcept>
#include "core/iterminal.hpp"

namespace DofM
{

    class WindowsTerminal : public ITerminal
    {
    public:
        WindowsTerminal() : ITerminal() {}
        ~WindowsTerminal() {}
        void SetupNonBlockingTerminal();
        void ClearScreen();
        void ScanKeyboardInput(std::shared_ptr<std::vector<char> > outdata);
        void ReadPlatformNativeTerminalSize(unsigned short &maxrow, unsigned short &maxcol);
    private:
        HANDLE InHandle, OutHandle;
        DWORD OriginalInHandle, OriginalOutHandle;
        INPUT_RECORD ReadCharBuffer[16];
        DWORD ReadBufferBytesAvailable;
    };

} // DofM

#endif //DUNGONSOFMORGINAR_WINDOWSTERMINAL_HPP
