#pragma once

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::swing {
class KeyStroke final : public ::jxx::lang::ClassBase<KeyStroke,
    ::jxx::lang::Object, ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<KeyStroke, JxxSuper, ::jxx::io::SerializableI>;

    static ::jxx::Ptr<KeyStroke> getKeyStroke(::jxx::lang::jchar keyChar);
    static ::jxx::Ptr<KeyStroke> getKeyStroke(::jxx::lang::jint keyCode,
        ::jxx::lang::jint modifiers, ::jxx::lang::jbool onKeyRelease = false);
    static ::jxx::Ptr<KeyStroke> getKeyStrokeForEvent(
        ::jxx::lang::jint keyCode, ::jxx::lang::jchar keyChar,
        ::jxx::lang::jint modifiers, ::jxx::lang::jbool onKeyRelease);

    ::jxx::lang::jchar getKeyChar() const;
    ::jxx::lang::jint getKeyCode() const;
    ::jxx::lang::jint getModifiers() const;
    ::jxx::lang::jbool isOnKeyRelease() const;

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
    void readObjectNoData() override;

    KeyStroke(::jxx::lang::jint keyCode, ::jxx::lang::jchar keyChar,
        ::jxx::lang::jint modifiers, ::jxx::lang::jbool onKeyRelease);

private:
    ::jxx::lang::jint keyCode_ = 0;
    ::jxx::lang::jchar keyChar_ = 0;
    ::jxx::lang::jint modifiers_ = 0;
    ::jxx::lang::jbool onKeyRelease_ = false;
};
}
