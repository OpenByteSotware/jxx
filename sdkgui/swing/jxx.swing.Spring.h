#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::swing
{
    class Spring : public ::jxx::lang::ClassBase<Spring, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<Spring, JxxSuper>;
        static constexpr ::jxx::lang::jint UNSET =
            static_cast<::jxx::lang::jint>(0x80000000U);

        Spring(::jxx::lang::jint minimum, ::jxx::lang::jint preferred,
            ::jxx::lang::jint maximum);
        virtual ::jxx::lang::jint getMinimumValue() const;
        virtual ::jxx::lang::jint getPreferredValue() const;
        virtual ::jxx::lang::jint getMaximumValue() const;
        virtual ::jxx::lang::jint getValue() const;
        virtual void setValue(::jxx::lang::jint value);

        static ::jxx::Ptr<Spring> constant(::jxx::lang::jint preferred);
        static ::jxx::Ptr<Spring> constant(::jxx::lang::jint minimum,
            ::jxx::lang::jint preferred, ::jxx::lang::jint maximum);
        static ::jxx::Ptr<Spring> minus(const ::jxx::Ptr<Spring>& spring);
        static ::jxx::Ptr<Spring> sum(const ::jxx::Ptr<Spring>& first,
            const ::jxx::Ptr<Spring>& second);
        static ::jxx::Ptr<Spring> max(const ::jxx::Ptr<Spring>& first,
            const ::jxx::Ptr<Spring>& second);
        static ::jxx::Ptr<Spring> scale(const ::jxx::Ptr<Spring>& spring,
            ::jxx::lang::jfloat factor);
    private:
        ::jxx::lang::jint minimum_;
        ::jxx::lang::jint preferred_;
        ::jxx::lang::jint maximum_;
        ::jxx::lang::jint value_ = UNSET;
    };
}
