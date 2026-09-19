#include "swing/jxx.swing.JToolTip.h"
namespace jxx::swing
{
    JToolTip::JToolTip() : Super() {}
    void JToolTip::setTipText(const ::jxx::Ptr<::jxx::lang::String>& value)
    { tipText_ = value; }
    ::jxx::Ptr<::jxx::lang::String> JToolTip::getTipText() const
    { return tipText_; }
    void JToolTip::setComponent(const ::jxx::Ptr<JComponent>& value)
    { component_ = value; }
    ::jxx::Ptr<JComponent> JToolTip::getComponent() const
    { return component_; }
}
