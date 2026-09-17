#include "awt/jxx.awt.Canvas.h"

#include <sstream>

#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    Canvas::Canvas() : Canvas(nullptr) {}

    Canvas::Canvas(const ::jxx::Ptr<GraphicsConfiguration>& configuration)
        : graphicsConfiguration_(configuration), requestedBufferCount_(0)
    {
    }

    void Canvas::addNotify()
    {
        validate();
    }

    void Canvas::paint(const ::jxx::Ptr<Graphics>&)
    {
    }

    void Canvas::update(const ::jxx::Ptr<Graphics>& graphics)
    {
        paint(graphics);
    }

    void Canvas::createBufferStrategy(::jxx::lang::jint numberBuffers)
    {
        createBufferStrategy(numberBuffers, nullptr);
    }

    void Canvas::createBufferStrategy(::jxx::lang::jint numberBuffers,
        const ::jxx::Ptr<BufferCapabilities>& capabilities)
    {
        if (numberBuffers < 1)
            throw ::jxx::lang::IllegalArgumentException("numberBuffers");

        requestedBufferCount_ = numberBuffers;
        requestedBufferCapabilities_ = capabilities;
        bufferStrategy_.reset();
    }

    ::jxx::Ptr<::jxx::awt::image::BufferStrategy>
    Canvas::getBufferStrategy() const
    {
        return bufferStrategy_;
    }

    ::jxx::Ptr<GraphicsConfiguration>
    Canvas::getGraphicsConfiguration() const
    {
        return graphicsConfiguration_;
    }

    ::jxx::Ptr<::jxx::lang::String> Canvas::paramString() const
    {
        std::ostringstream out;
        out << "bufferCount=" << requestedBufferCount_;
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
