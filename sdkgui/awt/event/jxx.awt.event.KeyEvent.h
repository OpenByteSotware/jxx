#pragma once

#include "awt/event/jxx.awt.event.InputEvent.h"

namespace jxx::awt::event
{
    class KeyEvent : public ::jxx::lang::ClassBase<KeyEvent, InputEvent>
    {
    public:
        using JxxSuper = InputEvent;
        using Super = ::jxx::lang::ClassBase<KeyEvent, JxxSuper>;

        static constexpr ::jxx::lang::jint KEY_FIRST = 400;
        static constexpr ::jxx::lang::jint KEY_TYPED = 400;
        static constexpr ::jxx::lang::jint KEY_PRESSED = 401;
        static constexpr ::jxx::lang::jint KEY_RELEASED = 402;
        static constexpr ::jxx::lang::jint KEY_LAST = 402;
        static constexpr ::jxx::lang::jint VK_UNDEFINED = 0;
        static constexpr ::jxx::lang::jchar CHAR_UNDEFINED =
            static_cast<::jxx::lang::jchar>(0xffff);

        KeyEvent(const ::jxx::Ptr<::jxx::awt::Component>& source,
            ::jxx::lang::jint id, ::jxx::lang::jlong when,
            ::jxx::lang::jint modifiers, ::jxx::lang::jint keyCode,
            ::jxx::lang::jchar keyChar);
        ::jxx::lang::jint getKeyCode() const;
        void setKeyCode(::jxx::lang::jint keyCode);
        ::jxx::lang::jchar getKeyChar() const;
        void setKeyChar(::jxx::lang::jchar keyChar);

    protected:
        ::jxx::Ptr<::jxx::lang::String> paramString() const override;

    private:
        ::jxx::lang::jint keyCode_;
        ::jxx::lang::jchar keyChar_;
    };
}
