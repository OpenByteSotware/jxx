#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::awt
{
    class DisplayMode;
    class GraphicsConfiguration;
    class GraphicsConfigTemplate;
    class Window;

    class GraphicsDevice : public ::jxx::lang::ClassBase<GraphicsDevice,
        ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<GraphicsDevice, JxxSuper>;
        using ConfigurationArray = ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::Ptr<GraphicsConfiguration>, 1>>;
        using DisplayModeArray = ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::Ptr<DisplayMode>, 1>>;

        static constexpr ::jxx::lang::jint TYPE_RASTER_SCREEN = 0;
        static constexpr ::jxx::lang::jint TYPE_PRINTER = 1;
        static constexpr ::jxx::lang::jint TYPE_IMAGE_BUFFER = 2;

        ~GraphicsDevice() override = default;

        virtual ::jxx::lang::jint getType() const = 0;
        virtual ::jxx::Ptr<::jxx::lang::String> getIDstring() const = 0;
        virtual ConfigurationArray getConfigurations() const = 0;
        virtual ::jxx::Ptr<GraphicsConfiguration>
            getDefaultConfiguration() const = 0;
        virtual ::jxx::Ptr<GraphicsConfiguration> getBestConfiguration(
            const ::jxx::Ptr<GraphicsConfigTemplate>& templateValue) const;
        virtual ::jxx::lang::jbool isFullScreenSupported() const;
        virtual void setFullScreenWindow(const ::jxx::Ptr<Window>& window);
        virtual ::jxx::Ptr<Window> getFullScreenWindow() const;
        virtual ::jxx::lang::jbool isDisplayChangeSupported() const;
        virtual void setDisplayMode(const ::jxx::Ptr<DisplayMode>& mode);
        virtual ::jxx::Ptr<DisplayMode> getDisplayMode() const;
        virtual DisplayModeArray getDisplayModes() const;
        virtual ::jxx::lang::jint getAvailableAcceleratedMemory() const;

    protected:
        GraphicsDevice();

    private:
        ::jxx::Ptr<Window> fullScreenWindow_;
    };
}
