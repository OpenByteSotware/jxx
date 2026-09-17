#include "awt/jxx.awt.PointerInfo.h"

namespace jxx::awt
{
    PointerInfo::PointerInfo(const ::jxx::Ptr<GraphicsDevice>& device,
        const ::jxx::Ptr<Point>& location)
        : device_(device), location_(location != nullptr
            ? ::jxx::NEW<Point>(location) : ::jxx::NEW<Point>())
    {
    }

    ::jxx::Ptr<GraphicsDevice> PointerInfo::getDevice() const
    {
        return device_;
    }

    ::jxx::Ptr<Point> PointerInfo::getLocation() const
    {
        return ::jxx::NEW<Point>(location_);
    }
}
