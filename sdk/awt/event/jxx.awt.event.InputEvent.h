#pragma once

#include "awt/event/jxx.awt.event.ComponentEvent.h"

namespace jxx::awt::event
{
    class InputEvent :
        public ::jxx::lang::ClassBase<InputEvent, ComponentEvent>
    {
    public:
        using JxxSuper = ComponentEvent;
        using Super = ::jxx::lang::ClassBase<InputEvent, JxxSuper>;

        static constexpr ::jxx::lang::jint SHIFT_MASK = 1;
        static constexpr ::jxx::lang::jint CTRL_MASK = 2;
        static constexpr ::jxx::lang::jint META_MASK = 4;
        static constexpr ::jxx::lang::jint ALT_MASK = 8;
        static constexpr ::jxx::lang::jint BUTTON1_MASK = 16;
        static constexpr ::jxx::lang::jint ALT_GRAPH_MASK = 32;
        static constexpr ::jxx::lang::jint BUTTON2_MASK = 8;
        static constexpr ::jxx::lang::jint BUTTON3_MASK = 4;

        static constexpr ::jxx::lang::jint SHIFT_DOWN_MASK = 64;
        static constexpr ::jxx::lang::jint CTRL_DOWN_MASK = 128;
        static constexpr ::jxx::lang::jint META_DOWN_MASK = 256;
        static constexpr ::jxx::lang::jint ALT_DOWN_MASK = 512;
        static constexpr ::jxx::lang::jint BUTTON1_DOWN_MASK = 1024;
        static constexpr ::jxx::lang::jint BUTTON2_DOWN_MASK = 2048;
        static constexpr ::jxx::lang::jint BUTTON3_DOWN_MASK = 4096;
        static constexpr ::jxx::lang::jint ALT_GRAPH_DOWN_MASK = 8192;

        ::jxx::lang::jlong getWhen() const;
        ::jxx::lang::jint getModifiers() const;
        ::jxx::lang::jint getModifiersEx() const;
        ::jxx::lang::jbool isShiftDown() const;
        ::jxx::lang::jbool isControlDown() const;
        ::jxx::lang::jbool isMetaDown() const;
        ::jxx::lang::jbool isAltDown() const;
        ::jxx::lang::jbool isAltGraphDown() const;
        void consume();
        ::jxx::lang::jbool isConsumed() const;
        static ::jxx::lang::jint getMaskForButton(::jxx::lang::jint button);

    protected:
        InputEvent(const ::jxx::Ptr<::jxx::awt::Component>& source,
            ::jxx::lang::jint id, ::jxx::lang::jlong when,
            ::jxx::lang::jint modifiers);

    private:
        ::jxx::lang::jlong when_;
        ::jxx::lang::jint modifiers_;
    };
}
