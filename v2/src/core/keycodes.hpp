//
// Created by eh on 1/8/22.
//

#ifndef DUNGONSOFMORGINAR_KEYCODES_HPP
#define DUNGONSOFMORGINAR_KEYCODES_HPP
#include <string_view>
#include <vector>
#include <fmt/core.h>
#include <fmt/ranges.h>

namespace DofM
{
    struct KeyCodes
    {



        struct KeySequence
        {
            enum ASCII
            {
                _NULL = 0,
                BELL = 7,
                _BACKSP = 8,
                TAB = 9,
                LF = 10,
                VTAB = 11,
                FF = 12,
                CR = 13,
                CANCEL = 24,
                ESC = 27,
                FS = 28,
                CSI = 91,
                TILDE = 126,
                DEL = 127
            };

            typedef const std::vector<char> SEQ;

            static inline SEQ ESCAPE = {ASCII::ESC};
            static inline SEQ UP     = {ASCII::ESC, ASCII::CSI, 65};
            static inline SEQ DOWN   = {ASCII::ESC, ASCII::CSI, 66};
            static inline SEQ RIGHT  = {ASCII::ESC, ASCII::CSI, 67};
            static inline SEQ LEFT   = {ASCII::ESC, ASCII::CSI, 68};
            static inline SEQ END    = {ASCII::ESC, ASCII::CSI, 70};
            static inline SEQ HOME   = {ASCII::ESC, ASCII::CSI, 72};
            static inline SEQ F1     = {ASCII::ESC, ASCII::CSI, 49, 49, ASCII::TILDE};
            static inline SEQ F2     = {ASCII::ESC, ASCII::CSI, 49, 50, ASCII::TILDE};
            static inline SEQ F3     = {ASCII::ESC, ASCII::CSI, 49, 51, ASCII::TILDE};
            static inline SEQ F4     = {ASCII::ESC, ASCII::CSI, 49, 52, ASCII::TILDE};
            static inline SEQ F5     = {ASCII::ESC, ASCII::CSI, 49, 53, ASCII::TILDE};
            static inline SEQ F6     = {ASCII::ESC, ASCII::CSI, 49, 54, ASCII::TILDE};
            static inline SEQ F7     = {ASCII::ESC, ASCII::CSI, 49, 55, ASCII::TILDE};
            static inline SEQ F8     = {ASCII::ESC, ASCII::CSI, 49, 56, ASCII::TILDE};
            static inline SEQ INSERT = {ASCII::ESC, ASCII::CSI, 50, ASCII::TILDE};
            static inline SEQ DELETE = {ASCII::ESC, ASCII::CSI, 51, ASCII::TILDE};
            static inline SEQ BKSPACE= {ASCII::DEL};
            static inline SEQ RETURN = {ASCII::LF};

            static inline SEQ PG_UP    = {ASCII::ESC, ASCII::CSI, 53, ASCII::TILDE};
            static inline SEQ PG_DOWN  = {ASCII::ESC, ASCII::CSI, 54, ASCII::TILDE};


            // 27 91 54 126

        };

        enum KeyPress
        {
            UP, DOWN, RIGHT, LEFT,
            HOME, END,
            PG_UP, PG_DOWN,
            INSERT, DELETE, BACKSPACE,
            RETURN,
            F1, F2, F3, F4, F5, F6, F7, F8,
            ESC,
            ALPHA,
            NUMBER,
            SPECIAL_CHARACTER,
            NO_KEY = 9999
        };



        static KeyPress MatchKey(const std::vector<char> &ksq)
        {
            if (ksq == KeySequence::ESCAPE) return KeyPress::ESC;
            else if (ksq == KeySequence::UP) return KeyPress::UP;
            else if (ksq == KeySequence::DOWN) return KeyPress::DOWN;
            else if (ksq == KeySequence::RIGHT) return KeyPress::RIGHT;
            else if (ksq == KeySequence::LEFT) return KeyPress::LEFT;
            else if (ksq == KeySequence::PG_UP) return KeyPress::PG_UP;
            else if (ksq == KeySequence::PG_DOWN) return KeyPress::PG_DOWN;
            else if (ksq == KeySequence::END) return KeyPress::END;
            else if (ksq == KeySequence::HOME) return KeyPress::HOME;

            else if (ksq == KeySequence::F1) return KeyPress::F1;
            else if (ksq == KeySequence::F2) return KeyPress::F2;
            else if (ksq == KeySequence::F3) return KeyPress::F3;
            else if (ksq == KeySequence::F4) return KeyPress::F4;
            else if (ksq == KeySequence::F5) return KeyPress::F5;
            else if (ksq == KeySequence::F6) return KeyPress::F6;
            else if (ksq == KeySequence::F7) return KeyPress::F7;
            else if (ksq == KeySequence::F8) return KeyPress::F8;

            else if (ksq == KeySequence::INSERT) return KeyPress::INSERT;
            else if (ksq == KeySequence::DELETE) return KeyPress::DELETE;
            else if (ksq == KeySequence::BKSPACE) return KeyPress::BACKSPACE;
            else if (ksq == KeySequence::RETURN) return KeyPress::RETURN;
            else if (IsLetter(ksq))
                return KeyPress::ALPHA;
            else if (IsNumber(ksq))
                return KeyPress::NUMBER;
            else if (IsSpecialCharacters(ksq))
                return KeyPress::SPECIAL_CHARACTER;
            else return KeyPress::NO_KEY;
        }

        static char ValidateAndReturnSingleChar(const std::vector<char> &keys)
        {
            if (keys.size() > 1 || keys.empty())
            {
                std::cout << fmt::format("ERROR IN: {}", fmt::join(keys, "|")) << std::endl;
                throw std::invalid_argument("We are getting a empty or sequence into key/number validation");
            }
            return keys[0];
        }

        static bool IsSpecialCharacters(const std::vector<char> &keys)
        {
            auto n = ValidateAndReturnSingleChar(keys);
            if ( ((int)n >=  32  && (int)n <=  47) ||
                 ((int)n >=  58  && (int)n <=  64) ||
                 ((int)n >=  91  && (int)n <=  96) ||
                 ((int)n >= 123  && (int)n <= 126) )
            {
                return true;
            }
            return false;
        }

        static bool IsNumber(const std::vector<char> &keys)
        {

            auto n = ValidateAndReturnSingleChar(keys);
            if ((int)n >= 48 && (int)n <= 57)
            {
                return true;
            }
            return false;
        }

        static bool IsLetter(const std::vector<char> &keys)
        {
            auto n = ValidateAndReturnSingleChar(keys);
            if ( ((int)n >= 97 && (int)n <= 122) ||
                    ((int)n >= 65 && (int)n <= 90))
            {
                return true;
            }
            return false;
        }



        static constexpr bool IsKeyCode(char &incodesequence, unsigned int keycode)
        {
            if (incodesequence == keycode)
                return true;
            return false;
        }
    };
}
#endif //DUNGONSOFMORGINAR_KEYCODES_HPP
