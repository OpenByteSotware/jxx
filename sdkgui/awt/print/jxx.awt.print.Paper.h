#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt::print
{
    class Paper : public ::jxx::lang::ClassBase<Paper, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<Paper, JxxSuper>;

        Paper();
        ::jxx::Ptr<::jxx::lang::Object> clone() const override;
        ::jxx::lang::jdouble getHeight() const;
        ::jxx::lang::jdouble getWidth() const;
        void setSize(::jxx::lang::jdouble width,
            ::jxx::lang::jdouble height);
        void setImageableArea(::jxx::lang::jdouble x,
            ::jxx::lang::jdouble y, ::jxx::lang::jdouble width,
            ::jxx::lang::jdouble height);
        ::jxx::lang::jdouble getImageableX() const;
        ::jxx::lang::jdouble getImageableY() const;
        ::jxx::lang::jdouble getImageableWidth() const;
        ::jxx::lang::jdouble getImageableHeight() const;

    private:
        ::jxx::lang::jdouble width_ = 612.0;
        ::jxx::lang::jdouble height_ = 792.0;
        ::jxx::lang::jdouble imageableX_ = 72.0;
        ::jxx::lang::jdouble imageableY_ = 72.0;
        ::jxx::lang::jdouble imageableWidth_ = 468.0;
        ::jxx::lang::jdouble imageableHeight_ = 648.0;
    };
}
