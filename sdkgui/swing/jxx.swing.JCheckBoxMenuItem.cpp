#include "swing/jxx.swing.JCheckBoxMenuItem.h"
namespace jxx::swing
{
    JCheckBoxMenuItem::JCheckBoxMenuItem() : Super() {}
    JCheckBoxMenuItem::JCheckBoxMenuItem(const ::jxx::Ptr<::jxx::lang::String>& text) : Super(text) {}
    JCheckBoxMenuItem::JCheckBoxMenuItem(const ::jxx::Ptr<::jxx::lang::String>& text, ::jxx::lang::jbool selected) : Super(text), selected_(selected) {}
    ::jxx::lang::jbool JCheckBoxMenuItem::getState() const { return selected_; }
    void JCheckBoxMenuItem::setState(::jxx::lang::jbool value) { selected_ = value; }
    ::jxx::lang::jbool JCheckBoxMenuItem::isSelected() const { return selected_; }
    void JCheckBoxMenuItem::setSelected(::jxx::lang::jbool value) { selected_ = value; }
}
