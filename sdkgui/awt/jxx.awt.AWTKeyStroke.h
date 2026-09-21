#pragma once

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt::event
{
    class KeyEvent;
}

namespace jxx::io
{
    class ObjectInputStream;
    class ObjectOutputStream;
}

namespace jxx::awt
{
    class AWTKeyStroke : public ::jxx::lang::ClassBase<AWTKeyStroke,
        ::jxx::lang::Object, ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<AWTKeyStroke, JxxSuper,
            ::jxx::io::SerializableI>;

        ~AWTKeyStroke() override = default;

        static ::jxx::Ptr<AWTKeyStroke> getAWTKeyStroke(
            ::jxx::lang::jchar keyChar);
        static ::jxx::Ptr<AWTKeyStroke> getAWTKeyStroke(
            ::jxx::lang::jchar keyChar,
            ::jxx::lang::jint modifiers);
        static ::jxx::Ptr<AWTKeyStroke> getAWTKeyStroke(
            ::jxx::lang::jint keyCode,
            ::jxx::lang::jint modifiers);
        static ::jxx::Ptr<AWTKeyStroke> getAWTKeyStroke(
            ::jxx::lang::jint keyCode,
            ::jxx::lang::jint modifiers,
            ::jxx::lang::jbool onKeyRelease);
        static ::jxx::Ptr<AWTKeyStroke> getAWTKeyStrokeForEvent(
            const ::jxx::Ptr<::jxx::awt::event::KeyEvent>& event);

        ::jxx::lang::jchar getKeyChar() const;
        ::jxx::lang::jint getKeyCode() const;
        ::jxx::lang::jint getModifiers() const;
        ::jxx::lang::jbool isOnKeyRelease() const;
        ::jxx::lang::jint getKeyEventType() const;

        ::jxx::lang::jbool equals(
            const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
        ::jxx::lang::jint hashCode() const override;
        ::jxx::Ptr<::jxx::lang::String> toString() const override;

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

    protected:
        AWTKeyStroke();
        AWTKeyStroke(::jxx::lang::jchar keyChar,
            ::jxx::lang::jint keyCode,
            ::jxx::lang::jint modifiers,
            ::jxx::lang::jbool onKeyRelease);

    private:
        ::jxx::lang::jchar keyChar_;
        ::jxx::lang::jint keyCode_;
        ::jxx::lang::jint modifiers_;
        ::jxx::lang::jbool onKeyRelease_;
    };
}
