#pragma once

#include "awt/jxx.awt.Composite.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::awt
{
    class AlphaComposite final :
        public ::jxx::lang::ClassBase<AlphaComposite,
            ::jxx::lang::Object, Composite>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<AlphaComposite,
            JxxSuper, Composite>;

        static constexpr ::jxx::lang::jint CLEAR = 1;
        static constexpr ::jxx::lang::jint SRC = 2;
        static constexpr ::jxx::lang::jint DST = 9;
        static constexpr ::jxx::lang::jint SRC_OVER = 3;
        static constexpr ::jxx::lang::jint DST_OVER = 4;
        static constexpr ::jxx::lang::jint SRC_IN = 5;
        static constexpr ::jxx::lang::jint DST_IN = 6;
        static constexpr ::jxx::lang::jint SRC_OUT = 7;
        static constexpr ::jxx::lang::jint DST_OUT = 8;
        static constexpr ::jxx::lang::jint SRC_ATOP = 10;
        static constexpr ::jxx::lang::jint DST_ATOP = 11;
        static constexpr ::jxx::lang::jint XOR = 12;

        static ::jxx::Ptr<AlphaComposite> Clear;
        static ::jxx::Ptr<AlphaComposite> Src;
        static ::jxx::Ptr<AlphaComposite> Dst;
        static ::jxx::Ptr<AlphaComposite> SrcOver;
        static ::jxx::Ptr<AlphaComposite> DstOver;
        static ::jxx::Ptr<AlphaComposite> SrcIn;
        static ::jxx::Ptr<AlphaComposite> DstIn;
        static ::jxx::Ptr<AlphaComposite> SrcOut;
        static ::jxx::Ptr<AlphaComposite> DstOut;
        static ::jxx::Ptr<AlphaComposite> SrcAtop;
        static ::jxx::Ptr<AlphaComposite> DstAtop;
        static ::jxx::Ptr<AlphaComposite> Xor;

        static ::jxx::Ptr<AlphaComposite> getInstance(
            ::jxx::lang::jint rule);
        static ::jxx::Ptr<AlphaComposite> getInstance(
            ::jxx::lang::jint rule,
            ::jxx::lang::jfloat alpha);

        ::jxx::Ptr<AlphaComposite> derive(::jxx::lang::jint rule) const;
        ::jxx::Ptr<AlphaComposite> derive(::jxx::lang::jfloat alpha) const;
        ::jxx::lang::jfloat getAlpha() const;
        ::jxx::lang::jint getRule() const;
        ::jxx::Ptr<CompositeContext> createContext(
            const ::jxx::Ptr<::jxx::awt::image::ColorModel>& sourceColorModel,
            const ::jxx::Ptr<::jxx::awt::image::ColorModel>& destinationColorModel,
            const ::jxx::Ptr<RenderingHints>& hints) const override;
        ::jxx::lang::jbool equals(
            const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
        ::jxx::lang::jint hashCode() const override;

    private:
        AlphaComposite(::jxx::lang::jint rule,
            ::jxx::lang::jfloat alpha);
        static ::jxx::Ptr<AlphaComposite> create(
            ::jxx::lang::jint rule, ::jxx::lang::jfloat alpha);
        static void validate(::jxx::lang::jint rule,
            ::jxx::lang::jfloat alpha);

        ::jxx::lang::jint rule_;
        ::jxx::lang::jfloat alpha_;
    };
}
