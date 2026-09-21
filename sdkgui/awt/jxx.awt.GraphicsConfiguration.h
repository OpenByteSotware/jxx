#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt::geom
{
    class AffineTransform;
}

namespace jxx::awt::image
{
    class BufferedImage;
    class ColorModel;
    class VolatileImage;
}

namespace jxx::awt
{
    class BufferCapabilities;
    class GraphicsDevice;
    class ImageCapabilities;
    class Rectangle;

    class GraphicsConfiguration : public ::jxx::lang::ClassBase<
        GraphicsConfiguration, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<GraphicsConfiguration, JxxSuper>;

        ~GraphicsConfiguration() override = default;

        virtual ::jxx::Ptr<GraphicsDevice> getDevice() const = 0;
        virtual ::jxx::Ptr<::jxx::awt::image::ColorModel>
            getColorModel() const = 0;
        virtual ::jxx::Ptr<::jxx::awt::image::ColorModel> getColorModel(
            ::jxx::lang::jint transparency) const;
        virtual ::jxx::Ptr<::jxx::awt::geom::AffineTransform>
            getDefaultTransform() const = 0;
        virtual ::jxx::Ptr<::jxx::awt::geom::AffineTransform>
            getNormalizingTransform() const = 0;
        virtual ::jxx::Ptr<Rectangle> getBounds() const = 0;
        virtual ::jxx::Ptr<::jxx::awt::image::BufferedImage>
            createCompatibleImage(::jxx::lang::jint width,
                ::jxx::lang::jint height) const;
        virtual ::jxx::Ptr<::jxx::awt::image::BufferedImage>
            createCompatibleImage(::jxx::lang::jint width,
                ::jxx::lang::jint height,
                ::jxx::lang::jint transparency) const;
        virtual ::jxx::Ptr<::jxx::awt::image::VolatileImage>
            createCompatibleVolatileImage(::jxx::lang::jint width,
                ::jxx::lang::jint height) const;
        virtual ::jxx::Ptr<BufferCapabilities>
            getBufferCapabilities() const;
        virtual ::jxx::Ptr<ImageCapabilities>
            getImageCapabilities() const;
        virtual ::jxx::lang::jbool isTranslucencyCapable() const;

    protected:
        GraphicsConfiguration();
    };
}
