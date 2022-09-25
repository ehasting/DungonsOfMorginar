//
// Created by EgilHasting on 25/09/2022.
//

#ifndef DUNGONSOFMORGINAR_ITERMINAL_HPP
#define DUNGONSOFMORGINAR_ITERMINAL_HPP
#include <tuple>
#include <memory>
#include <vector>

namespace DofM
{

    class ITerminal
    {
    public:
        ITerminal() { }
        ~ITerminal() { }
        virtual void SetupNonBlockingTerminal() = 0;
        virtual void ClearScreen() = 0;
        virtual void ScanKeyboardInput(std::shared_ptr<std::vector<char> > outdata) = 0;
        virtual void ReadPlatformNativeTerminalSize(unsigned short &maxrow, unsigned short &maxcol) = 0;
    };

} // DofM

#endif //DUNGONSOFMORGINAR_ITERMINAL_HPP
