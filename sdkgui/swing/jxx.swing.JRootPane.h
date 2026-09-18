#pragma once

#include "swing/jxx.swing.JComponent.h"

namespace jxx::swing
{
    class AbstractButton;
    class JLayeredPane;

    class JRootPane :
        public ::jxx::lang::ClassBase<JRootPane, JComponent>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<JRootPane, JxxSuper>;

        static constexpr ::jxx::lang::jint NONE = 0;
        static constexpr ::jxx::lang::jint FRAME = 1;
        static constexpr ::jxx::lang::jint PLAIN_DIALOG = 2;
        static constexpr ::jxx::lang::jint INFORMATION_DIALOG = 3;
        static constexpr ::jxx::lang::jint ERROR_DIALOG = 4;
        static constexpr ::jxx::lang::jint COLOR_CHOOSER_DIALOG = 5;
        static constexpr ::jxx::lang::jint FILE_CHOOSER_DIALOG = 6;
        static constexpr ::jxx::lang::jint QUESTION_DIALOG = 7;
        static constexpr ::jxx::lang::jint WARNING_DIALOG = 8;

        JRootPane();
        ~JRootPane() override = default;

        void setContentPane(
            const ::jxx::Ptr<::jxx::awt::Container>& contentPane);
        ::jxx::Ptr<::jxx::awt::Container> getContentPane() const;
        void setLayeredPane(const ::jxx::Ptr<JLayeredPane>& layeredPane);
        ::jxx::Ptr<JLayeredPane> getLayeredPane() const;
        void setGlassPane(
            const ::jxx::Ptr<::jxx::awt::Component>& glassPane);
        ::jxx::Ptr<::jxx::awt::Component> getGlassPane() const;
        void setDefaultButton(
            const ::jxx::Ptr<AbstractButton>& defaultButton);
        ::jxx::Ptr<AbstractButton> getDefaultButton() const;
        void setWindowDecorationStyle(::jxx::lang::jint style);
        ::jxx::lang::jint getWindowDecorationStyle() const;

    private:
        ::jxx::Ptr<::jxx::awt::Container> contentPane_;
        ::jxx::Ptr<JLayeredPane> layeredPane_;
        ::jxx::Ptr<::jxx::awt::Component> glassPane_;
        ::jxx::Ptr<AbstractButton> defaultButton_;
        ::jxx::lang::jint windowDecorationStyle_ = NONE;
    };
}
