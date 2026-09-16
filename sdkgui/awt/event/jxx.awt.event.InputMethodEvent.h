#pragma once

#include "awt/jxx.awt.AWTEvent.h"
#include "awt/jxx.awt.Component.h"

namespace jxx::awt::font
{
    class TextHitInfo;
}

namespace jxx::text
{
    class AttributedCharacterIterator;
}

namespace jxx::awt::event
{
    class InputMethodEvent :
        public ::jxx::lang::ClassBase<InputMethodEvent, ::jxx::awt::AWTEvent>
    {
    public:
        using JxxSuper = ::jxx::awt::AWTEvent;
        using Super = ::jxx::lang::ClassBase<InputMethodEvent, JxxSuper>;

        static constexpr ::jxx::lang::jint INPUT_METHOD_FIRST = 1100;
        static constexpr ::jxx::lang::jint INPUT_METHOD_TEXT_CHANGED = 1100;
        static constexpr ::jxx::lang::jint CARET_POSITION_CHANGED = 1101;
        static constexpr ::jxx::lang::jint INPUT_METHOD_LAST = 1101;

        InputMethodEvent(
            const ::jxx::Ptr<::jxx::awt::Component>& source,
            ::jxx::lang::jint id,
            const ::jxx::Ptr<::jxx::text::AttributedCharacterIterator>& text,
            ::jxx::lang::jint committedCharacterCount,
            const ::jxx::Ptr<::jxx::awt::font::TextHitInfo>& caret,
            const ::jxx::Ptr<::jxx::awt::font::TextHitInfo>& visiblePosition);

        InputMethodEvent(
            const ::jxx::Ptr<::jxx::awt::Component>& source,
            ::jxx::lang::jint id,
            ::jxx::lang::jlong when,
            const ::jxx::Ptr<::jxx::text::AttributedCharacterIterator>& text,
            ::jxx::lang::jint committedCharacterCount,
            const ::jxx::Ptr<::jxx::awt::font::TextHitInfo>& caret,
            const ::jxx::Ptr<::jxx::awt::font::TextHitInfo>& visiblePosition);

        InputMethodEvent(
            const ::jxx::Ptr<::jxx::awt::Component>& source,
            ::jxx::lang::jint id,
            const ::jxx::Ptr<::jxx::awt::font::TextHitInfo>& caret,
            const ::jxx::Ptr<::jxx::awt::font::TextHitInfo>& visiblePosition);

        ::jxx::Ptr<::jxx::text::AttributedCharacterIterator> getText() const;
        ::jxx::lang::jint getCommittedCharacterCount() const;
        ::jxx::Ptr<::jxx::awt::font::TextHitInfo> getCaret() const;
        ::jxx::Ptr<::jxx::awt::font::TextHitInfo> getVisiblePosition() const;
        ::jxx::lang::jlong getWhen() const;
        void consume();
        ::jxx::lang::jbool isConsumed() const;

    protected:
        ::jxx::Ptr<::jxx::lang::String> paramString() const override;

    private:
        ::jxx::lang::jlong when_;
        ::jxx::Ptr<::jxx::text::AttributedCharacterIterator> text_;
        ::jxx::lang::jint committedCharacterCount_;
        ::jxx::Ptr<::jxx::awt::font::TextHitInfo> caret_;
        ::jxx::Ptr<::jxx::awt::font::TextHitInfo> visiblePosition_;
    };
}
