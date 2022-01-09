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
        static constexpr const char CK_LEFT[] { 27, 91, 68, 0};
        static constexpr const char CK_UP[] { 27, 91, 65, 0};
        static constexpr const char CK_RIGHT[] { 27, 91, 67, 0};
        static constexpr const char CK_DOWN[] { 27, 91, 66, 0};

        static constexpr const char F1[] { 27, 79, 80, 0};
        static constexpr const char F2[] { 27, 79, 81, 0};
        static constexpr const char F3[] { 27, 79, 82, 0};
        static constexpr const char F4[] { 27, 79, 83, 0};

        static constexpr bool IsKeyCode(std::string_view &incodesequence, const char *keycode)
        {
            if (incodesequence == keycode)
                return true;
            return false;
        }
    };
}
#endif //DUNGONSOFMORGINAR_KEYCODES_HPP
