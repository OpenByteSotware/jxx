#include "swing/jxx.swing.JRadioButtonMenuItem.h"
namespace jxx::swing
{
    JRadioButtonMenuItem::JRadioButtonMenuItem() : Super() {}
    JRadioButtonMenuItem::JRadioButtonMenuItem(const ::jxx::Ptr<::jxx::lang::String>& text) : Super(text) {}
    JRadioButtonMenuItem::JRadioButtonMenuItem(const ::jxx::Ptr<::jxx::lang::String>& text, ::jxx::lang::jbool selected) : Super(text), selected_(selected) {}
    ::jxx::lang::jbool JRadioButtonMenuItem::isSelected() const { return selected_; }
    void JRadioButtonMenuItem::setSelected(::jxx::lang::jbool value) { selected_ = value; }
}
