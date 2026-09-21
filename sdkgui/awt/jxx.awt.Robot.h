#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt::image
{
    class BufferedImage;
}

namespace jxx::awt
{
    class Color;
    class GraphicsDevice;
    class Rectangle;

    class Robot : public ::jxx::lang::ClassBase<Robot, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<Robot, JxxSuper>;

        Robot();
        explicit Robot(const ::jxx::Ptr<GraphicsDevice>& screen);

        void mouseMove(::jxx::lang::jint x, ::jxx::lang::jint y);
        void mousePress(::jxx::lang::jint buttons);
        void mouseRelease(::jxx::lang::jint buttons);
        void mouseWheel(::jxx::lang::jint wheelAmount);
        void keyPress(::jxx::lang::jint keyCode);
        void keyRelease(::jxx::lang::jint keyCode);
        ::jxx::Ptr<Color> getPixelColor(
            ::jxx::lang::jint x, ::jxx::lang::jint y) const;
        ::jxx::Ptr<::jxx::awt::image::BufferedImage> createScreenCapture(
            const ::jxx::Ptr<Rectangle>& screenRect) const;
        ::jxx::lang::jbool isAutoWaitForIdle() const;
        void setAutoWaitForIdle(::jxx::lang::jbool value);
        ::jxx::lang::jint getAutoDelay() const;
        void setAutoDelay(::jxx::lang::jint milliseconds);
        void delay(::jxx::lang::jint milliseconds) const;
        void waitForIdle() const;

    private:
        void afterEvent() const;
        ::jxx::Ptr<GraphicsDevice> screen_;
        ::jxx::lang::jbool autoWaitForIdle_ = false;
        ::jxx::lang::jint autoDelay_ = 0;
    };
}
