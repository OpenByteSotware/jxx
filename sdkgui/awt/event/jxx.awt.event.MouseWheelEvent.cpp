#include "awt/event/jxx.awt.event.MouseWheelEvent.h"

#include <sstream>

namespace jxx::awt::event
{
    MouseWheelEvent::MouseWheelEvent(
        const ::jxx::Ptr<::jxx::awt::Component>& source,
        ::jxx::lang::jint id, ::jxx::lang::jlong when,
        ::jxx::lang::jint modifiers, ::jxx::lang::jint x,
        ::jxx::lang::jint y, ::jxx::lang::jint clickCount,
        ::jxx::lang::jbool popupTrigger, ::jxx::lang::jint scrollType,
        ::jxx::lang::jint scrollAmount, ::jxx::lang::jint wheelRotation,
        ::jxx::lang::jdouble preciseWheelRotation)
        : Super(source, id, when, modifiers, x, y, clickCount,
            popupTrigger, NOBUTTON),
          scrollType_(scrollType), scrollAmount_(scrollAmount),
          wheelRotation_(wheelRotation),
          preciseWheelRotation_(preciseWheelRotation)
    {
    }

    ::jxx::lang::jint MouseWheelEvent::getScrollType() const { return scrollType_; }
    ::jxx::lang::jint MouseWheelEvent::getScrollAmount() const { return scrollAmount_; }
    ::jxx::lang::jint MouseWheelEvent::getWheelRotation() const { return wheelRotation_; }
    ::jxx::lang::jdouble MouseWheelEvent::getPreciseWheelRotation() const { return preciseWheelRotation_; }
    ::jxx::lang::jint MouseWheelEvent::getUnitsToScroll() const { return scrollAmount_ * wheelRotation_; }

    ::jxx::Ptr<::jxx::lang::String> MouseWheelEvent::paramString() const
    {
        std::ostringstream out;
        out << "MOUSE_WHEEL,scrollType=" << scrollType_
            << ",scrollAmount=" << scrollAmount_
            << ",wheelRotation=" << wheelRotation_
            << ",preciseWheelRotation=" << preciseWheelRotation_;
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
