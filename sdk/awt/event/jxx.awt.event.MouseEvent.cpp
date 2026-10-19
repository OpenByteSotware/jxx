#include "awt/event/jxx.awt.event.MouseEvent.h"

#include <sstream>

namespace jxx::awt::event
{
    MouseEvent::MouseEvent(const ::jxx::Ptr<::jxx::awt::Component>& source,
        ::jxx::lang::jint id, ::jxx::lang::jlong when,
        ::jxx::lang::jint modifiers, ::jxx::lang::jint x,
        ::jxx::lang::jint y, ::jxx::lang::jint clicks,
        ::jxx::lang::jbool popup, ::jxx::lang::jint button)
        : Super(source, id, when, modifiers), x_(x), y_(y),
          clickCount_(clicks), button_(button), popup_(popup)
    {
    }
    ::jxx::lang::jint MouseEvent::getX() const { return x_; }
    ::jxx::lang::jint MouseEvent::getY() const { return y_; }
    ::jxx::Ptr<::jxx::awt::Point> MouseEvent::getPoint() const { return ::jxx::NEW<::jxx::awt::Point>(x_, y_); }
    ::jxx::lang::jint MouseEvent::getClickCount() const { return clickCount_; }
    ::jxx::lang::jint MouseEvent::getButton() const { return button_; }
    ::jxx::lang::jbool MouseEvent::isPopupTrigger() const { return popup_; }
    ::jxx::Ptr<::jxx::lang::String> MouseEvent::paramString() const
    {
        std::ostringstream out;
        out << "MOUSE,id=" << id << ",x=" << x_ << ",y=" << y_
            << ",button=" << button_ << ",clickCount=" << clickCount_
            << ",modifiers=" << getModifiers();
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
