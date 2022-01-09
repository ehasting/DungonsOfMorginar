//
// Created by eh on 1/8/22.
//

#ifndef DUNGONSOFMORGINAR_KEYCODES_HPP
#define DUNGONSOFMORGINAR_KEYCODES_HPP
#include <string_view>
namespace DofM
{
    struct KeyCodes
    {
        enum EscapedKeys
        {
            UP = 65,
            DOWN = 66,
            RIGHT = 67,
            LEFT = 68
        };

        static EscapedKeys EvaluateEscapedKey(char &key)
        {
            return (EscapedKeys)key;
        }

        static constexpr const char F1[] { 27, 79, 80, 0};
        static constexpr const char F2[] { 27, 79, 81, 0};
        static constexpr const char F3[] { 27, 79, 82, 0};
        static constexpr const char F4[] { 27, 79, 83, 0}; // 27|91|49|53|126 ESC[15~

        static constexpr bool IsKeyCode(char &incodesequence, unsigned int keycode)
        {
            if (incodesequence == keycode)
                return true;
            return false;
        }
    };
}
#endif //DUNGONSOFMORGINAR_KEYCODES_HPP
