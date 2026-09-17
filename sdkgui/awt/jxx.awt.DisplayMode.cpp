#include "awt/jxx.awt.DisplayMode.h"

#include <cstdint>
#include <sstream>

namespace jxx::awt
{
    DisplayMode::DisplayMode(::jxx::lang::jint width,
        ::jxx::lang::jint height,
        ::jxx::lang::jint bitDepth,
        ::jxx::lang::jint refreshRate)
        : width_(width), height_(height), bitDepth_(bitDepth),
          refreshRate_(refreshRate)
    {
    }

    ::jxx::lang::jint DisplayMode::getWidth() const { return width_; }
    ::jxx::lang::jint DisplayMode::getHeight() const { return height_; }
    ::jxx::lang::jint DisplayMode::getBitDepth() const { return bitDepth_; }
    ::jxx::lang::jint DisplayMode::getRefreshRate() const { return refreshRate_; }

    ::jxx::lang::jbool DisplayMode::equals(
        const ::jxx::Ptr<DisplayMode>& mode) const
    {
        return mode != nullptr && width_ == mode->width_ &&
            height_ == mode->height_ && bitDepth_ == mode->bitDepth_ &&
            refreshRate_ == mode->refreshRate_;
    }

    ::jxx::lang::jbool DisplayMode::equals(
        const ::jxx::Ptr<::jxx::lang::Object>& object) const
    {
        return equals(::jxx::CAST<DisplayMode>(object));
    }

    ::jxx::lang::jint DisplayMode::hashCode() const
    {
        std::uint32_t value = static_cast<std::uint32_t>(width_);
        value = value * 31u + static_cast<std::uint32_t>(height_);
        value = value * 31u + static_cast<std::uint32_t>(bitDepth_);
        value = value * 31u + static_cast<std::uint32_t>(refreshRate_);
        return static_cast<::jxx::lang::jint>(value);
    }

    ::jxx::Ptr<::jxx::lang::String> DisplayMode::toString() const
    {
        std::ostringstream out;
        out << width_ << "x" << height_ << "x" << bitDepth_ << "@"
            << refreshRate_;
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
