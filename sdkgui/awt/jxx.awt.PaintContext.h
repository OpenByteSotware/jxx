#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::awt::image
{
    class ColorModel;
    class Raster;
}

namespace jxx::awt
{
    class PaintContext : public ::jxx::lang::InterfaceBase<PaintContext>
    {
    public:
        ~PaintContext() override = default;
        virtual void dispose() = 0;
        virtual ::jxx::Ptr<::jxx::awt::image::ColorModel>
            getColorModel() const = 0;
        virtual ::jxx::Ptr<::jxx::awt::image::Raster> getRaster(
            ::jxx::lang::jint x,
            ::jxx::lang::jint y,
            ::jxx::lang::jint width,
            ::jxx::lang::jint height) = 0;
    };
}
