#include "awt/jxx.awt.ImageCapabilities.h"

namespace jxx::awt
{
    ImageCapabilities::ImageCapabilities(::jxx::lang::jbool accelerated)
        : accelerated_(accelerated)
    {
    }

    ::jxx::lang::jbool ImageCapabilities::isAccelerated() const
    {
        return accelerated_;
    }

    ::jxx::lang::jbool ImageCapabilities::isTrueVolatile() const
    {
        return false;
    }

    ::jxx::Ptr<::jxx::lang::Object> ImageCapabilities::clone() const
    {
        return ::jxx::NEW<ImageCapabilities>(accelerated_);
    }
}
