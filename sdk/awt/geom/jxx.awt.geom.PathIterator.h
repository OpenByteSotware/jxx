#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace jxx::awt::geom
{
    class PathIterator : public ::jxx::lang::InterfaceBase<PathIterator>
    {
    public:
        static constexpr ::jxx::lang::jint WIND_EVEN_ODD = 0;
        static constexpr ::jxx::lang::jint WIND_NON_ZERO = 1;

        static constexpr ::jxx::lang::jint SEG_MOVETO = 0;
        static constexpr ::jxx::lang::jint SEG_LINETO = 1;
        static constexpr ::jxx::lang::jint SEG_QUADTO = 2;
        static constexpr ::jxx::lang::jint SEG_CUBICTO = 3;
        static constexpr ::jxx::lang::jint SEG_CLOSE = 4;

        ~PathIterator() override = default;

        virtual ::jxx::lang::jint getWindingRule() const = 0;
        virtual ::jxx::lang::jbool isDone() const = 0;
        virtual void next() = 0;

        virtual ::jxx::lang::jint currentSegment(
            const ::jxx::Ptr<::jxx::lang::JxxArray<
                ::jxx::lang::jfloat,
                1>>& coordinates) const = 0;

        virtual ::jxx::lang::jint currentSegment(
            const ::jxx::Ptr<::jxx::lang::JxxArray<
                ::jxx::lang::jdouble,
                1>>& coordinates) const = 0;
    };
}
