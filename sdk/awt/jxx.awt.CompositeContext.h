#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::awt::image
{
    class Raster;
    class WritableRaster;
}

namespace jxx::awt
{
    class CompositeContext :
        public ::jxx::lang::InterfaceBase<CompositeContext>
    {
    public:
        ~CompositeContext() override = default;
        virtual void dispose() = 0;
        virtual void compose(
            const ::jxx::Ptr<::jxx::awt::image::Raster>& source,
            const ::jxx::Ptr<::jxx::awt::image::Raster>& destinationIn,
            const ::jxx::Ptr<::jxx::awt::image::WritableRaster>& destinationOut) = 0;
    };
}
