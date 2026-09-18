#include "swing/jxx.swing.JCheckBox.h"
namespace jxx::swing
{
    JCheckBox::JCheckBox() : Super() {}
    JCheckBox::JCheckBox(const ::jxx::Ptr<::jxx::lang::String>& text) : Super(text) {}
    JCheckBox::JCheckBox(const ::jxx::Ptr<::jxx::lang::String>& text, ::jxx::lang::jbool selected) : Super(text, selected) {}
    void JCheckBox::setBorderPaintedFlat(::jxx::lang::jbool value) { borderPaintedFlat_ = value; }
    ::jxx::lang::jbool JCheckBox::isBorderPaintedFlat() const { return borderPaintedFlat_; }
}
