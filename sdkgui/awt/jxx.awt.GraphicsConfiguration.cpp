#include "awt/jxx.awt.GraphicsConfiguration.h"

#include "awt/jxx.awt.ImageCapabilities.h"
#include "awt/jxx.awt.Transparency.h"

namespace jxx::awt
{
    GraphicsConfiguration::GraphicsConfiguration() : Super()
    {
    }

    ::jxx::Ptr<::jxx::awt::image::ColorModel>
    GraphicsConfiguration::getColorModel(
        ::jxx::lang::jint transparency) const
    {
        if (transparency == Transparency::OPAQUE)
            return getColorModel();
        return nullptr;
    }

    ::jxx::Ptr<::jxx::awt::image::BufferedImage>
    GraphicsConfiguration::createCompatibleImage(
        ::jxx::lang::jint, ::jxx::lang::jint) const
    {
        return nullptr;
    }

    ::jxx::Ptr<::jxx::awt::image::BufferedImage>
    GraphicsConfiguration::createCompatibleImage(
        ::jxx::lang::jint, ::jxx::lang::jint,
        ::jxx::lang::jint) const
    {
        return nullptr;
    }

    ::jxx::Ptr<::jxx::awt::image::VolatileImage>
    GraphicsConfiguration::createCompatibleVolatileImage(
        ::jxx::lang::jint, ::jxx::lang::jint) const
    {
        return nullptr;
    }

    ::jxx::Ptr<BufferCapabilities>
    GraphicsConfiguration::getBufferCapabilities() const
    {
        return nullptr;
    }

    ::jxx::Ptr<ImageCapabilities>
    GraphicsConfiguration::getImageCapabilities() const
    {
        return ::jxx::NEW<ImageCapabilities>(false);
    }

    ::jxx::lang::jbool GraphicsConfiguration::isTranslucencyCapable() const
    {
        return getColorModel(Transparency::TRANSLUCENT) != nullptr;
    }
}
