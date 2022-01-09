//
// Created by eh on 1/9/22.
//

#ifndef DUNGONSOFMORGINAR_SCREENPOS_HPP
#define DUNGONSOFMORGINAR_SCREENPOS_HPP
#include "fmt/core.h"


namespace DofM
{
    class ScreenPos
    {
        unsigned int _col;
        unsigned int _row;
    public:
        ScreenPos()
        {
            this->_row = 1;
            this->_col = 1;
        }

        ScreenPos(unsigned int col, unsigned int row)
        {
            this->SetCol(col);
            this->SetRow(row);
        }

        unsigned int Row()
        {
            return this->_row;
        }

        unsigned int Col()
        {
            return this->_col;
        }

        void SetRow(unsigned int row)
        {
            if (row >= 1)
            {
                this->_row = row;
                return;
            }
            throw std::invalid_argument(fmt::format("row out of bound: {}", row));
        }

        void SetCol(unsigned int col)
        {
            if (col >= 1)
            {
                this->_col = col;
                return;
            }
            throw std::invalid_argument(fmt::format("col out of bound: {}", col));
        }

    };
}

#endif //DUNGONSOFMORGINAR_SCREENPOS_HPP
