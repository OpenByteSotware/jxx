#include "awt/jxx.awt.GraphicsDevice.h"

#include "awt/jxx.awt.GraphicsConfiguration.h"

namespace jxx::awt
{
    GraphicsDevice::GraphicsDevice() : Super()
    {
    }

    ::jxx::Ptr<GraphicsConfiguration>
    GraphicsDevice::getBestConfiguration(
        const ::jxx::Ptr<GraphicsConfigTemplate>&) const
    {
        return getDefaultConfiguration();
    }

    ::jxx::lang::jbool GraphicsDevice::isFullScreenSupported() const
    {
        return false;
    }

    void GraphicsDevice::setFullScreenWindow(
        const ::jxx::Ptr<Window>& window)
    {
        fullScreenWindow_ = window;
    }

    ::jxx::Ptr<Window> GraphicsDevice::getFullScreenWindow() const
    {
        return fullScreenWindow_;
    }

    ::jxx::lang::jbool GraphicsDevice::isDisplayChangeSupported() const
    {
        return false;
    }

    void GraphicsDevice::setDisplayMode(const ::jxx::Ptr<DisplayMode>&)
    {
    }

    ::jxx::Ptr<DisplayMode> GraphicsDevice::getDisplayMode() const
    {
        return nullptr;
    }

    GraphicsDevice::DisplayModeArray GraphicsDevice::getDisplayModes() const
    {
        return ::jxx::NEW<::jxx::lang::JxxArray<
            ::jxx::Ptr<DisplayMode>, 1>>(0);
    }

    ::jxx::lang::jint GraphicsDevice::getAvailableAcceleratedMemory() const
    {
        return -1;
    }
}
