#include "swing/jxx.swing.DefaultColorSelectionModel.h"
#include <algorithm>
#include "swing/event/jxx.swing.event.ChangeEvent.h"
#include "swing/event/jxx.swing.event.ChangeListener.h"
namespace jxx::swing
{
    DefaultColorSelectionModel::DefaultColorSelectionModel()
        : DefaultColorSelectionModel(::jxx::awt::Color::white) {}
    DefaultColorSelectionModel::DefaultColorSelectionModel(
        const ::jxx::Ptr<::jxx::awt::Color>& color)
        : color_(color ? color : ::jxx::awt::Color::white) {}
    ::jxx::Ptr<::jxx::awt::Color>
    DefaultColorSelectionModel::getSelectedColor() const { return color_; }
    void DefaultColorSelectionModel::setSelectedColor(
        const ::jxx::Ptr<::jxx::awt::Color>& value)
    {
        const auto next = value ? value : ::jxx::awt::Color::white;
        if (color_ == next || (color_ && color_->equals(next))) return;
        color_ = next;
        fireStateChanged();
    }
    void DefaultColorSelectionModel::addChangeListener(
        const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener)
    {
        if (listener && std::find(listeners_.begin(), listeners_.end(), listener)
            == listeners_.end()) listeners_.push_back(listener);
    }
    void DefaultColorSelectionModel::removeChangeListener(
        const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener)
    {
        listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), listener),
            listeners_.end());
    }
    void DefaultColorSelectionModel::fireStateChanged()
    {
        auto event = ::jxx::NEW<::jxx::swing::event::ChangeEvent>(
            ::jxx::CAST<::jxx::lang::Object>(thisPtr()));
        const auto copy = listeners_;
        for (const auto& listener : copy) if (listener) listener->stateChanged(event);
    }
}
