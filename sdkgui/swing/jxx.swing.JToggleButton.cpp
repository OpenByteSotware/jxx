#include "swing/jxx.swing.JToggleButton.h"
#include "swing/jxx.swing.ButtonGroup.h"
namespace jxx::swing
{
    JToggleButton::JToggleButton() : Super() {}
    JToggleButton::JToggleButton(const ::jxx::Ptr<::jxx::lang::String>& text) : Super(text) {}
    JToggleButton::JToggleButton(const ::jxx::Ptr<::jxx::lang::String>& text, ::jxx::lang::jbool selected) : Super(text), selected_(selected) {}
    ::jxx::lang::jbool JToggleButton::isSelected() const { return selected_; }
    void JToggleButton::setSelected(::jxx::lang::jbool value)
    {
        if (const auto group = group_.lock()) group->setSelected(::jxx::CAST<JToggleButton>(thisPtr()), value);
        else selected_ = value;
    }
    void JToggleButton::setSelectedFromGroup(::jxx::lang::jbool value) { selected_ = value; }
    void JToggleButton::doClick() { setSelected(!isSelected()); AbstractButton::doClick(); }
}
