#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::awt::print
{
    class Paper;

    class PageFormat : public ::jxx::lang::ClassBase<PageFormat,
        ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<PageFormat, JxxSuper>;
        using Matrix = ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::lang::jdouble, 1>>;

        static constexpr ::jxx::lang::jint LANDSCAPE = 0;
        static constexpr ::jxx::lang::jint PORTRAIT = 1;
        static constexpr ::jxx::lang::jint REVERSE_LANDSCAPE = 2;

        PageFormat();
        ::jxx::Ptr<::jxx::lang::Object> clone() const override;
        ::jxx::lang::jdouble getWidth() const;
        ::jxx::lang::jdouble getHeight() const;
        ::jxx::lang::jdouble getImageableX() const;
        ::jxx::lang::jdouble getImageableY() const;
        ::jxx::lang::jdouble getImageableWidth() const;
        ::jxx::lang::jdouble getImageableHeight() const;
        ::jxx::Ptr<Paper> getPaper() const;
        void setPaper(const ::jxx::Ptr<Paper>& paper);
        ::jxx::lang::jint getOrientation() const;
        void setOrientation(::jxx::lang::jint orientation);
        Matrix getMatrix() const;

    private:
        ::jxx::lang::jint orientation_ = PORTRAIT;
        ::jxx::Ptr<Paper> paper_;
    };
}
