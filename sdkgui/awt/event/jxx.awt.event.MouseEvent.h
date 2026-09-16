#pragma once

#include "awt/event/jxx.awt.event.InputEvent.h"
#include "awt/jxx.awt.Point.h"

namespace jxx::awt::event
{
    class MouseEvent : public ::jxx::lang::ClassBase<MouseEvent, InputEvent>
    {
    public:
        using JxxSuper = InputEvent;
        using Super = ::jxx::lang::ClassBase<MouseEvent, JxxSuper>;

        static constexpr ::jxx::lang::jint MOUSE_FIRST = 500;
        static constexpr ::jxx::lang::jint MOUSE_CLICKED = 500;
        static constexpr ::jxx::lang::jint MOUSE_PRESSED = 501;
        static constexpr ::jxx::lang::jint MOUSE_RELEASED = 502;
        static constexpr ::jxx::lang::jint MOUSE_MOVED = 503;
        static constexpr ::jxx::lang::jint MOUSE_ENTERED = 504;
        static constexpr ::jxx::lang::jint MOUSE_EXITED = 505;
        static constexpr ::jxx::lang::jint MOUSE_DRAGGED = 506;
        static constexpr ::jxx::lang::jint MOUSE_WHEEL = 507;
        static constexpr ::jxx::lang::jint MOUSE_LAST = 507;
        static constexpr ::jxx::lang::jint NOBUTTON = 0;
        static constexpr ::jxx::lang::jint BUTTON1 = 1;
        static constexpr ::jxx::lang::jint BUTTON2 = 2;
        static constexpr ::jxx::lang::jint BUTTON3 = 3;

        MouseEvent(const ::jxx::Ptr<::jxx::awt::Component>& source,
            ::jxx::lang::jint id, ::jxx::lang::jlong when,
            ::jxx::lang::jint modifiers, ::jxx::lang::jint x,
            ::jxx::lang::jint y, ::jxx::lang::jint clicks,
            ::jxx::lang::jbool popupTrigger, ::jxx::lang::jint button);
        ::jxx::lang::jint getX() const;
        ::jxx::lang::jint getY() const;
        ::jxx::Ptr<::jxx::awt::Point> getPoint() const;
        ::jxx::lang::jint getClickCount() const;
        ::jxx::lang::jint getButton() const;
        ::jxx::lang::jbool isPopupTrigger() const;

    protected:
        ::jxx::Ptr<::jxx::lang::String> paramString() const override;

    private:
        ::jxx::lang::jint x_;
        ::jxx::lang::jint y_;
        ::jxx::lang::jint clickCount_;
        ::jxx::lang::jint button_;
        ::jxx::lang::jbool popup_;
    };
}
