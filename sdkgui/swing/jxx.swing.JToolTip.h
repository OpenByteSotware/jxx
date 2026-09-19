#pragma once
#include "swing/jxx.swing.JComponent.h"
namespace jxx::swing
{
    class JToolTip : public ::jxx::lang::ClassBase<JToolTip, JComponent>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<JToolTip, JxxSuper>;
        JToolTip();
        void setTipText(const ::jxx::Ptr<::jxx::lang::String>& text);
        ::jxx::Ptr<::jxx::lang::String> getTipText() const;
        void setComponent(const ::jxx::Ptr<JComponent>& component);
        ::jxx::Ptr<JComponent> getComponent() const;
    private:
        ::jxx::Ptr<::jxx::lang::String> tipText_;
        ::jxx::Ptr<JComponent> component_;
    };
}
