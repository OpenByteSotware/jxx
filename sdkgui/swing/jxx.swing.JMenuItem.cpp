#include "swing/jxx.swing.JMenuItem.h"
namespace jxx::swing
{
    JMenuItem::JMenuItem() : Super() {}
    JMenuItem::JMenuItem(const ::jxx::Ptr<::jxx::lang::String>& text) : Super(text) {}
    void JMenuItem::setArmed(::jxx::lang::jbool value) { armed_ = value; }
    ::jxx::lang::jbool JMenuItem::isArmed() const { return armed_; }
    void JMenuItem::menuSelectionChanged(::jxx::lang::jbool value) { setArmed(value); }
}
