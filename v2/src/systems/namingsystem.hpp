//
// Created by eh on 1/5/22.
//

#ifndef DUNGONSOFMORGINAR_NAMINGSYSTEM_H
#define DUNGONSOFMORGINAR_NAMINGSYSTEM_H

#include <vector>
#include <string>
#include <algorithm>
#include "tools.hpp"

namespace DofM
{
class NamingSystem
{

    std::vector<std::string> NameList = {"James", "Bond", "Rambo", "T800", "Arnold", "Rocky", "Egi", "Tor", "Ståle", "Mathias", "Lucas", "Connor", "John", "MacClain", "Wick"};
    std::vector<std::string> TitleList = {"Sir", "Mr.", "Master", "Duke", "Lord", "His Honour", "Dr.", "His Eminence"};
    std::vector<std::string> SuffixList = {"the Great", "the Useless", "of Loosertown", "the Insignificant", "of Smalltown"};
    std::vector<std::string> TakenNames;
    DofM::Tools ToolsObject;
    std::string GetRandomItemFromVector(std::vector<std::string> &vec)
    {
        std::vector<int> tmp;
        return GetRandomItemFromVector(vec, tmp);
    }

    std::string GetRandomItemFromVector(std::vector<std::string> &vec, std::vector<int> &lastdice)
    {
        int dice = 0;
        int iters = this->ToolsObject.RndRange(1, 100);
        while (true)
        {
            dice = this->ToolsObject.RndRange(0, vec.size()-1);
            iters--;
            if (iters == 0)
            {
                if (std::find(lastdice.begin(), lastdice.end(), dice) != lastdice.end())
                {
                    // allready taken in the session
                    iters++;
                    continue;
                }
                // Unique and ok.
                break;
            }
        }
        lastdice.push_back(dice);
        return vec.at(dice);
    }
    public:
    NamingSystem()
    {

    }

    std::string GetRandomName()
    {
        int numofnames = this->ToolsObject.RndRange(1,2);
        bool includetitle = this->ToolsObject.RndBool();
        bool includesuffix = this->ToolsObject.RndBool();
        return this->GetName(numofnames, includetitle, includesuffix);
    }

    std::string GetName(int numofnames, bool includetitle, bool includesuffix)
    {
        while (true)
        {
            std::string rval;
            if (includetitle)
            {

                rval.append(this->GetRandomItemFromVector(TitleList));
                rval.append(" ");
            }
            std::vector<int> sessiondices;
            for (int x = 0; x < numofnames; x++)
            {
                rval.append(this->GetRandomItemFromVector(this->NameList, sessiondices));
                rval.append(" ");
            }

            if (includesuffix)
            {
                rval.append(this->GetRandomItemFromVector(SuffixList));
            }
            // trim the name
            this->ToolsObject.rtrim(rval);
            this->ToolsObject.ltrim(rval);

            // check if name is taken
            bool istaken = false;
            for (auto &n : TakenNames)
            {
                if (rval == n)
                {
                    istaken = true;
                    break;
                }
            }
            if (istaken)
            {
                continue;
            }

            this->TakenNames.push_back(rval);
            return rval;
        }
    }

};
}
/*
 * Room has objects
 * objects can be used for blocking in battle, and used by ai-controllerd chars in battle
 * objects can be picked up by characters and have properties
 */

#endif //DUNGONSOFMORGINAR_NAMINGSYSTEM_H
