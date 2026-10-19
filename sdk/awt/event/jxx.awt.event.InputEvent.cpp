#include "awt/event/jxx.awt.event.InputEvent.h"

#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt::event
{
    InputEvent::InputEvent(
        const ::jxx::Ptr<::jxx::awt::Component>& source,
        ::jxx::lang::jint id,
        ::jxx::lang::jlong when,
        ::jxx::lang::jint modifiers)
        : Super(source, id), when_(when), modifiers_(modifiers)
    {
    }

    ::jxx::lang::jlong InputEvent::getWhen() const { return when_; }
    ::jxx::lang::jint InputEvent::getModifiers() const { return modifiers_; }
    ::jxx::lang::jint InputEvent::getModifiersEx() const
    {
        return modifiers_ & (SHIFT_DOWN_MASK | CTRL_DOWN_MASK | META_DOWN_MASK |
            ALT_DOWN_MASK | BUTTON1_DOWN_MASK | BUTTON2_DOWN_MASK |
            BUTTON3_DOWN_MASK | ALT_GRAPH_DOWN_MASK);
    }
    ::jxx::lang::jbool InputEvent::isShiftDown() const { return (modifiers_ & (SHIFT_MASK | SHIFT_DOWN_MASK)) != 0; }
    ::jxx::lang::jbool InputEvent::isControlDown() const { return (modifiers_ & (CTRL_MASK | CTRL_DOWN_MASK)) != 0; }
    ::jxx::lang::jbool InputEvent::isMetaDown() const { return (modifiers_ & (META_MASK | META_DOWN_MASK)) != 0; }
    ::jxx::lang::jbool InputEvent::isAltDown() const { return (modifiers_ & (ALT_MASK | ALT_DOWN_MASK)) != 0; }
    ::jxx::lang::jbool InputEvent::isAltGraphDown() const { return (modifiers_ & (ALT_GRAPH_MASK | ALT_GRAPH_DOWN_MASK)) != 0; }
    void InputEvent::consume() { AWTEvent::consume(); }
    ::jxx::lang::jbool InputEvent::isConsumed() const { return AWTEvent::isConsumed(); }

    ::jxx::lang::jint InputEvent::getMaskForButton(::jxx::lang::jint button)
    {
        switch (button)
        {
            case 1: return BUTTON1_DOWN_MASK;
            case 2: return BUTTON2_DOWN_MASK;
            case 3: return BUTTON3_DOWN_MASK;
            default: throw ::jxx::lang::IllegalArgumentException("button");
        }
    }
}
