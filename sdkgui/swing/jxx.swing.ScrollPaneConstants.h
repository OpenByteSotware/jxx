#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::swing
{
    class ScrollPaneConstants : public ::jxx::lang::InterfaceBase<ScrollPaneConstants>
    {
    public:
        static constexpr ::jxx::lang::jint VERTICAL_SCROLLBAR_AS_NEEDED = 20;
        static constexpr ::jxx::lang::jint VERTICAL_SCROLLBAR_NEVER = 21;
        static constexpr ::jxx::lang::jint VERTICAL_SCROLLBAR_ALWAYS = 22;
        static constexpr ::jxx::lang::jint HORIZONTAL_SCROLLBAR_AS_NEEDED = 30;
        static constexpr ::jxx::lang::jint HORIZONTAL_SCROLLBAR_NEVER = 31;
        static constexpr ::jxx::lang::jint HORIZONTAL_SCROLLBAR_ALWAYS = 32;
        ~ScrollPaneConstants() override = default;
    };
}
