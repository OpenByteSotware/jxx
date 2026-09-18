#include "swing/jxx.swing.ButtonGroup.h"
#include <algorithm>
#include "swing/jxx.swing.JToggleButton.h"
namespace jxx::swing
{
    void ButtonGroup::add(const ::jxx::Ptr<JToggleButton>& button)
    {
        if (!button || std::find(buttons_.begin(), buttons_.end(), button) != buttons_.end()) return;
        buttons_.push_back(button); button->group_ = ::jxx::CAST<ButtonGroup>(thisPtr());
        if (button->isSelected()) setSelected(button, true);
    }
    void ButtonGroup::remove(const ::jxx::Ptr<JToggleButton>& button)
    {
        if (!button) return; buttons_.erase(std::remove(buttons_.begin(), buttons_.end(), button), buttons_.end());
        if (selection_ == button) selection_.reset(); button->group_.reset();
    }
    void ButtonGroup::clearSelection() { if (selection_) selection_->setSelectedFromGroup(false); selection_.reset(); }
    ::jxx::lang::jint ButtonGroup::getButtonCount() const { return static_cast<::jxx::lang::jint>(buttons_.size()); }
    ::jxx::Ptr<JToggleButton> ButtonGroup::getSelection() const { return selection_; }
    ::jxx::lang::jbool ButtonGroup::isSelected(const ::jxx::Ptr<JToggleButton>& button) const { return selection_ == button; }
    void ButtonGroup::setSelected(const ::jxx::Ptr<JToggleButton>& button, ::jxx::lang::jbool selected)
    {
        if (!button) return; if (!selected) { if (selection_ == button) clearSelection(); return; }
        if (selection_ && selection_ != button) selection_->setSelectedFromGroup(false);
        selection_ = button; selection_->setSelectedFromGroup(true);
    }
    void ButtonGroup::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&) {}
    void ButtonGroup::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&) {}
    void ButtonGroup::readObjectNoData() { buttons_.clear(); selection_.reset(); }
}
