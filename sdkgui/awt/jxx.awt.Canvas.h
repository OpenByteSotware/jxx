#pragma once

#include "awt/jxx.awt.Component.h"

namespace jxx::awt
{
    class BufferCapabilities;
    class Graphics;
    class GraphicsConfiguration;
}

namespace jxx::awt::image
{
    class BufferStrategy;
}

namespace jxx::awt
{
    class Canvas : public ::jxx::lang::ClassBase<Canvas, Component>
    {
    public:
        using JxxSuper = Component;
        using Super = ::jxx::lang::ClassBase<Canvas, JxxSuper>;

        Canvas();
        explicit Canvas(
            const ::jxx::Ptr<GraphicsConfiguration>& configuration);
        ~Canvas() override = default;

        virtual void addNotify();
        virtual void paint(const ::jxx::Ptr<Graphics>& graphics);
        virtual void update(const ::jxx::Ptr<Graphics>& graphics);
        virtual void createBufferStrategy(::jxx::lang::jint numberBuffers);
        virtual void createBufferStrategy(::jxx::lang::jint numberBuffers,
            const ::jxx::Ptr<BufferCapabilities>& capabilities);
        virtual ::jxx::Ptr<::jxx::awt::image::BufferStrategy>
            getBufferStrategy() const;
        virtual ::jxx::Ptr<GraphicsConfiguration>
            getGraphicsConfiguration() const;

    protected:
        ::jxx::Ptr<::jxx::lang::String> paramString() const;

    private:
        ::jxx::Ptr<GraphicsConfiguration> graphicsConfiguration_;
        ::jxx::Ptr<::jxx::awt::image::BufferStrategy> bufferStrategy_;
        ::jxx::lang::jint requestedBufferCount_;
        ::jxx::Ptr<BufferCapabilities> requestedBufferCapabilities_;
    };
}
