#pragma once

#include "awt/event/jxx.awt.event.MouseEvent.h"

namespace jxx::awt::event
{
    class MouseWheelEvent :
        public ::jxx::lang::ClassBase<MouseWheelEvent, MouseEvent>
    {
    public:
        using JxxSuper = MouseEvent;
        using Super = ::jxx::lang::ClassBase<MouseWheelEvent, JxxSuper>;

        static constexpr ::jxx::lang::jint WHEEL_UNIT_SCROLL = 0;
        static constexpr ::jxx::lang::jint WHEEL_BLOCK_SCROLL = 1;

        MouseWheelEvent(const ::jxx::Ptr<::jxx::awt::Component>& source,
            ::jxx::lang::jint id, ::jxx::lang::jlong when,
            ::jxx::lang::jint modifiers, ::jxx::lang::jint x,
            ::jxx::lang::jint y, ::jxx::lang::jint clickCount,
            ::jxx::lang::jbool popupTrigger, ::jxx::lang::jint scrollType,
            ::jxx::lang::jint scrollAmount, ::jxx::lang::jint wheelRotation,
            ::jxx::lang::jdouble preciseWheelRotation);

        ::jxx::lang::jint getScrollType() const;
        ::jxx::lang::jint getScrollAmount() const;
        ::jxx::lang::jint getWheelRotation() const;
        ::jxx::lang::jdouble getPreciseWheelRotation() const;
        ::jxx::lang::jint getUnitsToScroll() const;

    protected:
        ::jxx::Ptr<::jxx::lang::String> paramString() const override;

    private:
        ::jxx::lang::jint scrollType_;
        ::jxx::lang::jint scrollAmount_;
        ::jxx::lang::jint wheelRotation_;
        ::jxx::lang::jdouble preciseWheelRotation_;
    };
}
