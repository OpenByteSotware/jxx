#include "awt/jxx.awt.AWTKeyStroke.h"

#include <sstream>

#include "awt/event/jxx.awt.event.KeyEvent.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"

namespace jxx::awt
{
    namespace
    {
        class AWTKeyStrokeNode final : public AWTKeyStroke
        {
        public:
            AWTKeyStrokeNode(::jxx::lang::jchar keyChar,
                ::jxx::lang::jint keyCode,
                ::jxx::lang::jint modifiers,
                ::jxx::lang::jbool onKeyRelease)
                : AWTKeyStroke(keyChar, keyCode, modifiers, onKeyRelease)
            {
            }
        };
    }

    AWTKeyStroke::AWTKeyStroke()
        : Super(),
          keyChar_(::jxx::awt::event::KeyEvent::CHAR_UNDEFINED),
          keyCode_(::jxx::awt::event::KeyEvent::VK_UNDEFINED),
          modifiers_(0), onKeyRelease_(false)
    {
    }

    AWTKeyStroke::AWTKeyStroke(::jxx::lang::jchar keyChar,
        ::jxx::lang::jint keyCode, ::jxx::lang::jint modifiers,
        ::jxx::lang::jbool onKeyRelease)
        : Super(), keyChar_(keyChar), keyCode_(keyCode),
          modifiers_(modifiers), onKeyRelease_(onKeyRelease)
    {
    }

    ::jxx::Ptr<AWTKeyStroke> AWTKeyStroke::getAWTKeyStroke(
        ::jxx::lang::jchar keyChar)
    {
        return ::jxx::NEW<AWTKeyStrokeNode>(keyChar,
            ::jxx::awt::event::KeyEvent::VK_UNDEFINED, 0, false);
    }

    ::jxx::Ptr<AWTKeyStroke> AWTKeyStroke::getAWTKeyStroke(
        ::jxx::lang::jchar keyChar, ::jxx::lang::jint modifiers)
    {
        return ::jxx::NEW<AWTKeyStrokeNode>(keyChar,
            ::jxx::awt::event::KeyEvent::VK_UNDEFINED, modifiers, false);
    }

    ::jxx::Ptr<AWTKeyStroke> AWTKeyStroke::getAWTKeyStroke(
        ::jxx::lang::jint keyCode, ::jxx::lang::jint modifiers)
    {
        return getAWTKeyStroke(keyCode, modifiers, false);
    }

    ::jxx::Ptr<AWTKeyStroke> AWTKeyStroke::getAWTKeyStroke(
        ::jxx::lang::jint keyCode, ::jxx::lang::jint modifiers,
        ::jxx::lang::jbool onKeyRelease)
    {
        return ::jxx::NEW<AWTKeyStrokeNode>(
            ::jxx::awt::event::KeyEvent::CHAR_UNDEFINED,
            keyCode, modifiers, onKeyRelease);
    }

    ::jxx::Ptr<AWTKeyStroke> AWTKeyStroke::getAWTKeyStrokeForEvent(
        const ::jxx::Ptr<::jxx::awt::event::KeyEvent>& event)
    {
        if (event == nullptr)
            return nullptr;
        if (event->getID() == ::jxx::awt::event::KeyEvent::KEY_TYPED)
            return getAWTKeyStroke(event->getKeyChar(),
                event->getModifiers());
        return getAWTKeyStroke(event->getKeyCode(), event->getModifiers(),
            event->getID() == ::jxx::awt::event::KeyEvent::KEY_RELEASED);
    }

    ::jxx::lang::jchar AWTKeyStroke::getKeyChar() const{return keyChar_;}
    ::jxx::lang::jint AWTKeyStroke::getKeyCode() const{return keyCode_;}
    ::jxx::lang::jint AWTKeyStroke::getModifiers() const{return modifiers_;}
    ::jxx::lang::jbool AWTKeyStroke::isOnKeyRelease() const{return onKeyRelease_;}
    ::jxx::lang::jint AWTKeyStroke::getKeyEventType() const
    {
        if (keyCode_ == ::jxx::awt::event::KeyEvent::VK_UNDEFINED)
            return ::jxx::awt::event::KeyEvent::KEY_TYPED;
        return onKeyRelease_ ?
            ::jxx::awt::event::KeyEvent::KEY_RELEASED :
            ::jxx::awt::event::KeyEvent::KEY_PRESSED;
    }

    ::jxx::lang::jbool AWTKeyStroke::equals(
        const ::jxx::Ptr<::jxx::lang::Object>& object) const
    {
        const auto other = ::jxx::CAST<AWTKeyStroke>(object);
        return other != nullptr && keyChar_ == other->keyChar_ &&
            keyCode_ == other->keyCode_ &&
            modifiers_ == other->modifiers_ &&
            onKeyRelease_ == other->onKeyRelease_;
    }

    ::jxx::lang::jint AWTKeyStroke::hashCode() const
    {
        return keyCode_ ^ (static_cast<::jxx::lang::jint>(keyChar_) << 16) ^
            modifiers_ ^ (onKeyRelease_ ? 1 : 0);
    }

    ::jxx::Ptr<::jxx::lang::String> AWTKeyStroke::toString() const
    {
        std::ostringstream out;
        out << "AWTKeyStroke[keyCode=" << keyCode_
            << ",keyChar=" << static_cast<unsigned int>(keyChar_)
            << ",modifiers=" << modifiers_
            << ",onKeyRelease=" << (onKeyRelease_ ? "true" : "false")
            << "]";
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }

    void AWTKeyStroke::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out)
    {
        out->writeInt(static_cast<::jxx::lang::jint>(keyChar_));
        out->writeInt(keyCode_); out->writeInt(modifiers_);
        out->writeBoolean(onKeyRelease_);
    }

    void AWTKeyStroke::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in)
    {
        keyChar_ = static_cast<::jxx::lang::jchar>(in->readInt());
        keyCode_ = in->readInt(); modifiers_ = in->readInt();
        onKeyRelease_ = in->readBoolean();
    }

    void AWTKeyStroke::readObjectNoData()
    {
        keyChar_ = ::jxx::awt::event::KeyEvent::CHAR_UNDEFINED;
        keyCode_ = ::jxx::awt::event::KeyEvent::VK_UNDEFINED;
        modifiers_ = 0; onKeyRelease_ = false;
    }
}
