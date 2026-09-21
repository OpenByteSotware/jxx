#pragma once

#include <vector>

#include "awt/print/jxx.awt.print.Pageable.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt::print
{
    class Book : public ::jxx::lang::ClassBase<Book,
        ::jxx::lang::Object, Pageable>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<Book, JxxSuper, Pageable>;

        Book();
        ::jxx::lang::jint getNumberOfPages() const override;
        ::jxx::Ptr<PageFormat> getPageFormat(
            ::jxx::lang::jint pageIndex) const override;
        ::jxx::Ptr<Printable> getPrintable(
            ::jxx::lang::jint pageIndex) const override;
        void setPage(::jxx::lang::jint pageIndex,
            const ::jxx::Ptr<Printable>& painter,
            const ::jxx::Ptr<PageFormat>& page);
        void append(const ::jxx::Ptr<Printable>& painter,
            const ::jxx::Ptr<PageFormat>& page);
        void append(const ::jxx::Ptr<Printable>& painter,
            const ::jxx::Ptr<PageFormat>& page,
            ::jxx::lang::jint numberOfPages);

    private:
        struct Entry
        {
            ::jxx::Ptr<Printable> painter;
            ::jxx::Ptr<PageFormat> format;
        };
        std::vector<Entry> pages_;
        const Entry& pageAt(::jxx::lang::jint pageIndex) const;
    };
}
