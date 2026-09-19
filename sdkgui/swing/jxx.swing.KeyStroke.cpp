#include "swing/jxx.swing.KeyStroke.h"

namespace jxx::swing {
KeyStroke::KeyStroke(::jxx::lang::jint keyCode, ::jxx::lang::jchar keyChar,
    ::jxx::lang::jint modifiers, ::jxx::lang::jbool onKeyRelease)
    : keyCode_(keyCode), keyChar_(keyChar), modifiers_(modifiers),
      onKeyRelease_(onKeyRelease) {}
::jxx::Ptr<KeyStroke> KeyStroke::getKeyStroke(::jxx::lang::jchar keyChar) {
    return ::jxx::NEW<KeyStroke>(0, keyChar, 0, false);
}
::jxx::Ptr<KeyStroke> KeyStroke::getKeyStroke(::jxx::lang::jint keyCode,
    ::jxx::lang::jint modifiers, ::jxx::lang::jbool onKeyRelease) {
    return ::jxx::NEW<KeyStroke>(keyCode, 0, modifiers, onKeyRelease);
}
::jxx::Ptr<KeyStroke> KeyStroke::getKeyStrokeForEvent(::jxx::lang::jint keyCode,
    ::jxx::lang::jchar keyChar, ::jxx::lang::jint modifiers,
    ::jxx::lang::jbool onKeyRelease) {
    return ::jxx::NEW<KeyStroke>(keyCode, keyChar, modifiers, onKeyRelease);
}
::jxx::lang::jchar KeyStroke::getKeyChar() const { return keyChar_; }
::jxx::lang::jint KeyStroke::getKeyCode() const { return keyCode_; }
::jxx::lang::jint KeyStroke::getModifiers() const { return modifiers_; }
::jxx::lang::jbool KeyStroke::isOnKeyRelease() const { return onKeyRelease_; }
void KeyStroke::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&) {}
void KeyStroke::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&) {}
void KeyStroke::readObjectNoData() {}
}
