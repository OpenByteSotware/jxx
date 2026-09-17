#pragma once

#include "awt/jxx.awt.Color.h"
#include "io/jxx.io.SerializableI.h"

namespace jxx::awt
{
    class SystemColor final :
        public ::jxx::lang::ClassBase<SystemColor, Color,
            ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = Color;
        using Super = ::jxx::lang::ClassBase<SystemColor, JxxSuper,
            ::jxx::io::SerializableI>;

        static constexpr ::jxx::lang::jint DESKTOP = 0;
        static constexpr ::jxx::lang::jint ACTIVE_CAPTION = 1;
        static constexpr ::jxx::lang::jint ACTIVE_CAPTION_TEXT = 2;
        static constexpr ::jxx::lang::jint ACTIVE_CAPTION_BORDER = 3;
        static constexpr ::jxx::lang::jint INACTIVE_CAPTION = 4;
        static constexpr ::jxx::lang::jint INACTIVE_CAPTION_TEXT = 5;
        static constexpr ::jxx::lang::jint INACTIVE_CAPTION_BORDER = 6;
        static constexpr ::jxx::lang::jint WINDOW = 7;
        static constexpr ::jxx::lang::jint WINDOW_BORDER = 8;
        static constexpr ::jxx::lang::jint WINDOW_TEXT = 9;
        static constexpr ::jxx::lang::jint MENU = 10;
        static constexpr ::jxx::lang::jint MENU_TEXT = 11;
        static constexpr ::jxx::lang::jint TEXT = 12;
        static constexpr ::jxx::lang::jint TEXT_TEXT = 13;
        static constexpr ::jxx::lang::jint TEXT_HIGHLIGHT = 14;
        static constexpr ::jxx::lang::jint TEXT_HIGHLIGHT_TEXT = 15;
        static constexpr ::jxx::lang::jint TEXT_INACTIVE_TEXT = 16;
        static constexpr ::jxx::lang::jint CONTROL = 17;
        static constexpr ::jxx::lang::jint CONTROL_TEXT = 18;
        static constexpr ::jxx::lang::jint CONTROL_HIGHLIGHT = 19;
        static constexpr ::jxx::lang::jint CONTROL_LT_HIGHLIGHT = 20;
        static constexpr ::jxx::lang::jint CONTROL_SHADOW = 21;
        static constexpr ::jxx::lang::jint CONTROL_DK_SHADOW = 22;
        static constexpr ::jxx::lang::jint SCROLLBAR = 23;
        static constexpr ::jxx::lang::jint INFO = 24;
        static constexpr ::jxx::lang::jint INFO_TEXT = 25;
        static constexpr ::jxx::lang::jint NUM_COLORS = 26;

        static ::jxx::Ptr<SystemColor> desktop;
        static ::jxx::Ptr<SystemColor> activeCaption;
        static ::jxx::Ptr<SystemColor> activeCaptionText;
        static ::jxx::Ptr<SystemColor> activeCaptionBorder;
        static ::jxx::Ptr<SystemColor> inactiveCaption;
        static ::jxx::Ptr<SystemColor> inactiveCaptionText;
        static ::jxx::Ptr<SystemColor> inactiveCaptionBorder;
        static ::jxx::Ptr<SystemColor> window;
        static ::jxx::Ptr<SystemColor> windowBorder;
        static ::jxx::Ptr<SystemColor> windowText;
        static ::jxx::Ptr<SystemColor> menu;
        static ::jxx::Ptr<SystemColor> menuText;
        static ::jxx::Ptr<SystemColor> text;
        static ::jxx::Ptr<SystemColor> textText;
        static ::jxx::Ptr<SystemColor> textHighlight;
        static ::jxx::Ptr<SystemColor> textHighlightText;
        static ::jxx::Ptr<SystemColor> textInactiveText;
        static ::jxx::Ptr<SystemColor> control;
        static ::jxx::Ptr<SystemColor> controlText;
        static ::jxx::Ptr<SystemColor> controlHighlight;
        static ::jxx::Ptr<SystemColor> controlLtHighlight;
        static ::jxx::Ptr<SystemColor> controlShadow;
        static ::jxx::Ptr<SystemColor> controlDkShadow;
        static ::jxx::Ptr<SystemColor> scrollbar;
        static ::jxx::Ptr<SystemColor> info;
        static ::jxx::Ptr<SystemColor> infoText;

        ::jxx::lang::jint getRGB() const;
        ::jxx::Ptr<::jxx::lang::String> toString() const override;
        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

        explicit SystemColor(::jxx::lang::jint index);

    private:
        static ::jxx::Ptr<SystemColor> create(::jxx::lang::jint index);
        static ::jxx::lang::jint rgbFor(::jxx::lang::jint index);
        ::jxx::lang::jint index_;
    };
}
