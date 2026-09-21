#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::awt
{
    class Graphics;
}

namespace jxx::awt::print
{
    class PageFormat;

    class Printable : public ::jxx::lang::InterfaceBase<Printable>
    {
    public:
        static constexpr ::jxx::lang::jint PAGE_EXISTS = 0;
        static constexpr ::jxx::lang::jint NO_SUCH_PAGE = 1;

        ~Printable() override = default;

        virtual ::jxx::lang::jint print(
            const ::jxx::Ptr<::jxx::awt::Graphics>& graphics,
            const ::jxx::Ptr<PageFormat>& pageFormat,
            ::jxx::lang::jint pageIndex) = 0;
    };
}
