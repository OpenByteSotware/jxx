#include "awt/jxx.awt.Robot.h"

#include <chrono>
#include <thread>

#include "awt/jxx.awt.Color.h"
#include "awt/jxx.awt.Rectangle.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    Robot::Robot() : Super()
    {
    }

    Robot::Robot(const ::jxx::Ptr<GraphicsDevice>& screen)
        : Super(), screen_(screen)
    {
    }

    void Robot::afterEvent() const
    {
        if (autoDelay_ > 0) delay(autoDelay_);
        if (autoWaitForIdle_) waitForIdle();
    }

    void Robot::mouseMove(::jxx::lang::jint, ::jxx::lang::jint){afterEvent();}
    void Robot::mousePress(::jxx::lang::jint){afterEvent();}
    void Robot::mouseRelease(::jxx::lang::jint){afterEvent();}
    void Robot::mouseWheel(::jxx::lang::jint){afterEvent();}
    void Robot::keyPress(::jxx::lang::jint){afterEvent();}
    void Robot::keyRelease(::jxx::lang::jint){afterEvent();}

    ::jxx::Ptr<Color> Robot::getPixelColor(
        ::jxx::lang::jint, ::jxx::lang::jint) const
    {
        return ::jxx::NEW<Color>(0, 0, 0);
    }

    ::jxx::Ptr<::jxx::awt::image::BufferedImage>
    Robot::createScreenCapture(const ::jxx::Ptr<Rectangle>& screenRect) const
    {
        if (screenRect == nullptr || screenRect->width <= 0 ||
            screenRect->height <= 0)
            throw ::jxx::lang::IllegalArgumentException("screenRect");
        return nullptr;
    }

    ::jxx::lang::jbool Robot::isAutoWaitForIdle() const{return autoWaitForIdle_;}
    void Robot::setAutoWaitForIdle(::jxx::lang::jbool value){autoWaitForIdle_=value;}
    ::jxx::lang::jint Robot::getAutoDelay() const{return autoDelay_;}
    void Robot::setAutoDelay(::jxx::lang::jint value)
    {
        if (value < 0 || value > 60000)
            throw ::jxx::lang::IllegalArgumentException("milliseconds");
        autoDelay_ = value;
    }
    void Robot::delay(::jxx::lang::jint value) const
    {
        if (value < 0 || value > 60000)
            throw ::jxx::lang::IllegalArgumentException("milliseconds");
        std::this_thread::sleep_for(std::chrono::milliseconds(value));
    }
    void Robot::waitForIdle() const{}
}
