#pragma once
#include "swing/jxx.swing.JComponent.h"
#include "swing/jxx.swing.SwingConstants.h"
#include "awt/jxx.awt.Insets.h"
namespace jxx::swing
{
    class AbstractButton;
    class JToolBar : public ::jxx::lang::ClassBase<JToolBar, JComponent, SwingConstants>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<JToolBar, JxxSuper, SwingConstants>;
        JToolBar();
        explicit JToolBar(::jxx::lang::jint orientation);
        explicit JToolBar(const ::jxx::Ptr<::jxx::lang::String>& name);
        JToolBar(const ::jxx::Ptr<::jxx::lang::String>& name, ::jxx::lang::jint orientation);
        ::jxx::Ptr<::jxx::awt::Component> add(const ::jxx::Ptr<::jxx::awt::Component>& component) override;
        ::jxx::Ptr<AbstractButton> add(const ::jxx::Ptr<AbstractButton>& button);
        void addSeparator();
        void addSeparator(const ::jxx::Ptr<::jxx::awt::Dimension>& size);
        void setOrientation(::jxx::lang::jint orientation);
        ::jxx::lang::jint getOrientation() const;
        void setFloatable(::jxx::lang::jbool value);
        ::jxx::lang::jbool isFloatable() const;
        void setRollover(::jxx::lang::jbool value);
        ::jxx::lang::jbool isRollover() const;
        void setBorderPainted(::jxx::lang::jbool value);
        ::jxx::lang::jbool isBorderPainted() const;
        void setMargin(const ::jxx::Ptr<::jxx::awt::Insets>& margin);
        ::jxx::Ptr<::jxx::awt::Insets> getMargin() const;
    private:
        ::jxx::lang::jint orientation_ = HORIZONTAL;
        ::jxx::lang::jbool floatable_ = true;
        ::jxx::lang::jbool rollover_ = false;
        ::jxx::lang::jbool borderPainted_ = true;
        ::jxx::Ptr<::jxx::awt::Insets> margin_;
    };
}
