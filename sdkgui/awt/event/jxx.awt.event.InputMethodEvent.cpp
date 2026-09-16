#include "awt/event/jxx.awt.event.InputMethodEvent.h"

#include <chrono>
#include <sstream>

namespace jxx::awt::event
{
    namespace
    {
        ::jxx::lang::jlong currentTimeMillis()
        {
            using namespace std::chrono;
            return static_cast<::jxx::lang::jlong>(
                duration_cast<milliseconds>(
                    system_clock::now().time_since_epoch()).count());
        }
    }

    InputMethodEvent::InputMethodEvent(
        const ::jxx::Ptr<::jxx::awt::Component>& source,
        ::jxx::lang::jint id,
        const ::jxx::Ptr<::jxx::text::AttributedCharacterIterator>& text,
        ::jxx::lang::jint committedCharacterCount,
        const ::jxx::Ptr<::jxx::awt::font::TextHitInfo>& caret,
        const ::jxx::Ptr<::jxx::awt::font::TextHitInfo>& visiblePosition)
        : InputMethodEvent(source, id, currentTimeMillis(), text,
              committedCharacterCount, caret, visiblePosition)
    {
    }

    InputMethodEvent::InputMethodEvent(
        const ::jxx::Ptr<::jxx::awt::Component>& source,
        ::jxx::lang::jint id,
        ::jxx::lang::jlong when,
        const ::jxx::Ptr<::jxx::text::AttributedCharacterIterator>& text,
        ::jxx::lang::jint committedCharacterCount,
        const ::jxx::Ptr<::jxx::awt::font::TextHitInfo>& caret,
        const ::jxx::Ptr<::jxx::awt::font::TextHitInfo>& visiblePosition)
        : Super(source, id),
          when_(when),
          text_(text),
          committedCharacterCount_(committedCharacterCount),
          caret_(caret),
          visiblePosition_(visiblePosition)
    {
    }

    InputMethodEvent::InputMethodEvent(
        const ::jxx::Ptr<::jxx::awt::Component>& source,
        ::jxx::lang::jint id,
        const ::jxx::Ptr<::jxx::awt::font::TextHitInfo>& caret,
        const ::jxx::Ptr<::jxx::awt::font::TextHitInfo>& visiblePosition)
        : InputMethodEvent(source, id, currentTimeMillis(), nullptr, 0,
              caret, visiblePosition)
    {
    }

    ::jxx::Ptr<::jxx::text::AttributedCharacterIterator>
    InputMethodEvent::getText() const
    {
        return text_;
    }

    ::jxx::lang::jint InputMethodEvent::getCommittedCharacterCount() const
    {
        return committedCharacterCount_;
    }

    ::jxx::Ptr<::jxx::awt::font::TextHitInfo>
    InputMethodEvent::getCaret() const
    {
        return caret_;
    }

    ::jxx::Ptr<::jxx::awt::font::TextHitInfo>
    InputMethodEvent::getVisiblePosition() const
    {
        return visiblePosition_;
    }

    ::jxx::lang::jlong InputMethodEvent::getWhen() const
    {
        return when_;
    }

    void InputMethodEvent::consume()
    {
        AWTEvent::consume();
    }

    ::jxx::lang::jbool InputMethodEvent::isConsumed() const
    {
        return AWTEvent::isConsumed();
    }

    ::jxx::Ptr<::jxx::lang::String> InputMethodEvent::paramString() const
    {
        std::ostringstream out;
        out << (id == INPUT_METHOD_TEXT_CHANGED
                ? "INPUT_METHOD_TEXT_CHANGED"
                : id == CARET_POSITION_CHANGED
                    ? "CARET_POSITION_CHANGED"
                    : "unknown type")
            << ",when=" << when_
            << ",committedCharacterCount=" << committedCharacterCount_;
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
