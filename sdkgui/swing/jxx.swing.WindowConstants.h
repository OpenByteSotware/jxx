#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::swing
{
    class WindowConstants : public ::jxx::lang::InterfaceBase<WindowConstants>
    {
    public:
        static constexpr ::jxx::lang::jint DO_NOTHING_ON_CLOSE = 0;
        static constexpr ::jxx::lang::jint HIDE_ON_CLOSE = 1;
        static constexpr ::jxx::lang::jint DISPOSE_ON_CLOSE = 2;
        static constexpr ::jxx::lang::jint EXIT_ON_CLOSE = 3;
        ~WindowConstants() override = default;
    };
}
