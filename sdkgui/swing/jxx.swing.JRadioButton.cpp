#include "swing/jxx.swing.JRadioButton.h"
namespace jxx::swing
{
    JRadioButton::JRadioButton() : Super() {}
    JRadioButton::JRadioButton(const ::jxx::Ptr<::jxx::lang::String>& text) : Super(text) {}
    JRadioButton::JRadioButton(const ::jxx::Ptr<::jxx::lang::String>& text, ::jxx::lang::jbool selected) : Super(text, selected) {}
}
