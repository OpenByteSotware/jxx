#include "swing/jxx.swing.JRootPane.h"

#include "awt/jxx.awt.BorderLayout.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "swing/jxx.swing.AbstractButton.h"
#include "swing/jxx.swing.JLayeredPane.h"
#include "swing/jxx.swing.JPanel.h"

namespace jxx::swing
{
    JRootPane::JRootPane()
        : Super(),
          contentPane_(::jxx::NEW<JPanel>(
              ::jxx::NEW<::jxx::awt::BorderLayout>())),
          layeredPane_(::jxx::NEW<JLayeredPane>()),
          glassPane_(::jxx::NEW<JPanel>())
    {
        glassPane_->setVisible(false);
        layeredPane_->add(contentPane_);
        Super::add(layeredPane_);
        Super::add(glassPane_);
    }

    void JRootPane::setContentPane(
        const ::jxx::Ptr<::jxx::awt::Container>& value)
    {
        if (value == nullptr)
            throw ::jxx::lang::NullPointerException("contentPane");
        if (contentPane_ != nullptr) layeredPane_->remove(contentPane_);
        contentPane_ = value;
        layeredPane_->add(contentPane_);
        invalidate();
    }

    ::jxx::Ptr<::jxx::awt::Container> JRootPane::getContentPane() const
    {
        return contentPane_;
    }

    void JRootPane::setLayeredPane(const ::jxx::Ptr<JLayeredPane>& value)
    {
        if (value == nullptr)
            throw ::jxx::lang::NullPointerException("layeredPane");
        if (layeredPane_ != nullptr) Super::remove(layeredPane_);
        layeredPane_ = value;
        layeredPane_->add(contentPane_);
        Super::add(layeredPane_);
        invalidate();
    }

    ::jxx::Ptr<JLayeredPane> JRootPane::getLayeredPane() const
    {
        return layeredPane_;
    }

    void JRootPane::setGlassPane(
        const ::jxx::Ptr<::jxx::awt::Component>& value)
    {
        if (value == nullptr)
            throw ::jxx::lang::NullPointerException("glassPane");
        if (glassPane_ != nullptr) Super::remove(glassPane_);
        glassPane_ = value;
        Super::add(glassPane_);
        invalidate();
    }

    ::jxx::Ptr<::jxx::awt::Component> JRootPane::getGlassPane() const
    {
        return glassPane_;
    }

    void JRootPane::setDefaultButton(
        const ::jxx::Ptr<AbstractButton>& value)
    {
        defaultButton_ = value;
    }

    ::jxx::Ptr<AbstractButton> JRootPane::getDefaultButton() const
    {
        return defaultButton_;
    }

    void JRootPane::setWindowDecorationStyle(::jxx::lang::jint value)
    {
        if (value < NONE || value > WARNING_DIALOG)
            throw ::jxx::lang::IllegalArgumentException(
                "windowDecorationStyle");
        windowDecorationStyle_ = value;
    }

    ::jxx::lang::jint JRootPane::getWindowDecorationStyle() const
    {
        return windowDecorationStyle_;
    }
}
