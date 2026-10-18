#pragma once

#include <vector>

#include "awt/jxx.awt.Stroke.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace jxx::awt
{
    class BasicStroke final :
        public ::jxx::lang::ClassBase<BasicStroke,
            ::jxx::lang::Object, Stroke>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<BasicStroke, JxxSuper, Stroke>;
        using FloatArray = ::jxx::Ptr<
            ::jxx::lang::JxxArray<::jxx::lang::jfloat, 1>>;

        static constexpr ::jxx::lang::jint JOIN_MITER = 0;
        static constexpr ::jxx::lang::jint JOIN_ROUND = 1;
        static constexpr ::jxx::lang::jint JOIN_BEVEL = 2;
        static constexpr ::jxx::lang::jint CAP_BUTT = 0;
        static constexpr ::jxx::lang::jint CAP_ROUND = 1;
        static constexpr ::jxx::lang::jint CAP_SQUARE = 2;

        BasicStroke();
        explicit BasicStroke(::jxx::lang::jfloat width);
        BasicStroke(::jxx::lang::jfloat width,
            ::jxx::lang::jint cap, ::jxx::lang::jint join);
        BasicStroke(::jxx::lang::jfloat width,
            ::jxx::lang::jint cap, ::jxx::lang::jint join,
            ::jxx::lang::jfloat miterLimit);
        BasicStroke(::jxx::lang::jfloat width,
            ::jxx::lang::jint cap, ::jxx::lang::jint join,
            ::jxx::lang::jfloat miterLimit,
            const FloatArray& dash,
            ::jxx::lang::jfloat dashPhase);

        ::jxx::Ptr<Shape> createStrokedShape(
            const ::jxx::Ptr<Shape>& shape) const override;
        ::jxx::lang::jfloat getLineWidth() const;
        ::jxx::lang::jint getEndCap() const;
        ::jxx::lang::jint getLineJoin() const;
        ::jxx::lang::jfloat getMiterLimit() const;
        FloatArray getDashArray() const;
        ::jxx::lang::jfloat getDashPhase() const;
        ::jxx::lang::jbool equals(
            const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
        ::jxx::lang::jint hashCode() const override;

    private:
        static void validate(::jxx::lang::jfloat width,
            ::jxx::lang::jint cap, ::jxx::lang::jint join,
            ::jxx::lang::jfloat miterLimit,
            const FloatArray& dash, ::jxx::lang::jfloat dashPhase);
        ::jxx::lang::jfloat width_;
        ::jxx::lang::jint cap_;
        ::jxx::lang::jint join_;
        ::jxx::lang::jfloat miterLimit_;
        std::vector<::jxx::lang::jfloat> dash_;
        ::jxx::lang::jfloat dashPhase_;
    };
}
