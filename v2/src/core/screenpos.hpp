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
            this->_row = 0;
            this->_col = 0;
        }

        ScreenPos(unsigned int col, unsigned int row)
        {
            this->SetCol(col);
            this->SetRow(row);
        }
        ScreenPos AddOffset(ScreenPos &offset)
        {
            ScreenPos rval(_col + offset.Col(), _row + offset.Row());
            return rval;
        }

        std::string ToString()
        {
            return fmt::format("{} x {}", this->_col, this->_row);
        }

        unsigned int Row()
        {
            return this->_row;
        }

        unsigned int Col()
        {
            return this->_col;
        }

        void IncreaseRow()
        {
           this->_row++;
        }

        void SetRow(unsigned int row)
        {
            if (row >= 0)
            {
                this->_row = row;
                return;
            }
            // we should check for screen max min instead
            throw std::invalid_argument(fmt::format("row out of bound: {}", row));
        }

        void SetCol(unsigned int col)
        {
            if (col >= 0)
            {
                this->_col = col;
                return;
            }
            throw std::invalid_argument(fmt::format("col out of bound: {}", col));
        }

    };
}

#endif //DUNGONSOFMORGINAR_SCREENPOS_HPP
