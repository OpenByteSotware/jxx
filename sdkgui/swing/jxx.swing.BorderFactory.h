#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx::awt
{
    class Color;
}

namespace jxx::swing::border
{
    class Border;
}

namespace jxx::swing
{
    class BorderFactory final
    {
    public:
        static ::jxx::Ptr<::jxx::swing::border::Border>
            createEmptyBorder();
        static ::jxx::Ptr<::jxx::swing::border::Border>
            createEmptyBorder(
                ::jxx::lang::jint top,
                ::jxx::lang::jint left,
                ::jxx::lang::jint bottom,
                ::jxx::lang::jint right);
        static ::jxx::Ptr<::jxx::swing::border::Border>
            createLineBorder(
                const ::jxx::Ptr<::jxx::awt::Color>& color);
        static ::jxx::Ptr<::jxx::swing::border::Border>
            createLineBorder(
                const ::jxx::Ptr<::jxx::awt::Color>& color,
                ::jxx::lang::jint thickness);
        static ::jxx::Ptr<::jxx::swing::border::Border>
            createLineBorder(
                const ::jxx::Ptr<::jxx::awt::Color>& color,
                ::jxx::lang::jint thickness,
                ::jxx::lang::jbool roundedCorners);
        static ::jxx::Ptr<::jxx::swing::border::Border>
            createCompoundBorder();
        static ::jxx::Ptr<::jxx::swing::border::Border>
            createCompoundBorder(
                const ::jxx::Ptr<::jxx::swing::border::Border>& outside,
                const ::jxx::Ptr<::jxx::swing::border::Border>& inside);

    private:
        BorderFactory() = delete;
    };
}
