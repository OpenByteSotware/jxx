#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::awt::print
{
    class PageFormat;
    class Printable;

    class Pageable : public ::jxx::lang::InterfaceBase<Pageable>
    {
    public:
        static constexpr ::jxx::lang::jint UNKNOWN_NUMBER_OF_PAGES = -1;

        ~Pageable() override = default;

        virtual ::jxx::lang::jint getNumberOfPages() const = 0;
        virtual ::jxx::Ptr<PageFormat> getPageFormat(
            ::jxx::lang::jint pageIndex) const = 0;
        virtual ::jxx::Ptr<Printable> getPrintable(
            ::jxx::lang::jint pageIndex) const = 0;
    };
}
