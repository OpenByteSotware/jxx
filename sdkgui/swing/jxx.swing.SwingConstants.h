#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::swing
{
    class SwingConstants : public ::jxx::lang::InterfaceBase<SwingConstants>
    {
    public:
        static constexpr ::jxx::lang::jint CENTER = 0;
        static constexpr ::jxx::lang::jint TOP = 1;
        static constexpr ::jxx::lang::jint LEFT = 2;
        static constexpr ::jxx::lang::jint BOTTOM = 3;
        static constexpr ::jxx::lang::jint RIGHT = 4;
        static constexpr ::jxx::lang::jint NORTH = 1;
        static constexpr ::jxx::lang::jint NORTH_EAST = 2;
        static constexpr ::jxx::lang::jint EAST = 3;
        static constexpr ::jxx::lang::jint SOUTH_EAST = 4;
        static constexpr ::jxx::lang::jint SOUTH = 5;
        static constexpr ::jxx::lang::jint SOUTH_WEST = 6;
        static constexpr ::jxx::lang::jint WEST = 7;
        static constexpr ::jxx::lang::jint NORTH_WEST = 8;
        static constexpr ::jxx::lang::jint HORIZONTAL = 0;
        static constexpr ::jxx::lang::jint VERTICAL = 1;
        static constexpr ::jxx::lang::jint LEADING = 10;
        static constexpr ::jxx::lang::jint TRAILING = 11;
        static constexpr ::jxx::lang::jint NEXT = 12;
        static constexpr ::jxx::lang::jint PREVIOUS = 13;
        ~SwingConstants() override = default;
    };
}
