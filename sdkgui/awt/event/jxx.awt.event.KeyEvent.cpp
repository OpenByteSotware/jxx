#include "awt/event/jxx.awt.event.KeyEvent.h"

#include <sstream>

namespace jxx::awt::event
{
    KeyEvent::KeyEvent(const ::jxx::Ptr<::jxx::awt::Component>& source,
        ::jxx::lang::jint id, ::jxx::lang::jlong when,
        ::jxx::lang::jint modifiers, ::jxx::lang::jint keyCode,
        ::jxx::lang::jchar keyChar)
        : Super(source, id, when, modifiers), keyCode_(keyCode), keyChar_(keyChar)
    {
    }
    ::jxx::lang::jint KeyEvent::getKeyCode() const { return keyCode_; }
    void KeyEvent::setKeyCode(::jxx::lang::jint value) { keyCode_ = value; }
    ::jxx::lang::jchar KeyEvent::getKeyChar() const { return keyChar_; }
    void KeyEvent::setKeyChar(::jxx::lang::jchar value) { keyChar_ = value; }
    ::jxx::Ptr<::jxx::lang::String> KeyEvent::paramString() const
    {
        const char* name = id == KEY_TYPED ? "KEY_TYPED" : id == KEY_PRESSED ? "KEY_PRESSED" : id == KEY_RELEASED ? "KEY_RELEASED" : "unknown type";
        std::ostringstream out;
        out << name << ",keyCode=" << keyCode_ << ",keyChar="
            << static_cast<unsigned int>(keyChar_) << ",modifiers=" << getModifiers();
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
