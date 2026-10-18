#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::awt::image
{
    class ColorModel;
}

namespace jxx::awt
{
    class CompositeContext;
    class RenderingHints;

    class Composite : public ::jxx::lang::InterfaceBase<Composite>
    {
    public:
        ~Composite() override = default;
        virtual ::jxx::Ptr<CompositeContext> createContext(
            const ::jxx::Ptr<::jxx::awt::image::ColorModel>& sourceColorModel,
            const ::jxx::Ptr<::jxx::awt::image::ColorModel>& destinationColorModel,
            const ::jxx::Ptr<RenderingHints>& hints) const = 0;
    };
}
