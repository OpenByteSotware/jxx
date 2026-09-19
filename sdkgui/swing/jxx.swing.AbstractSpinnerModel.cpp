#include "swing/jxx.swing.AbstractSpinnerModel.h"
#include <algorithm>
#include "swing/event/jxx.swing.event.ChangeEvent.h"
namespace jxx::swing
{
    void AbstractSpinnerModel::addChangeListener(const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener)
    { if (listener) listeners_.push_back(listener); }
    void AbstractSpinnerModel::removeChangeListener(const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener)
    { listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), listener), listeners_.end()); }
    void AbstractSpinnerModel::fireStateChanged()
    {
        auto event = ::jxx::NEW<::jxx::swing::event::ChangeEvent>(::jxx::CAST<::jxx::lang::Object>(thisPtr()));
        const auto copy = listeners_;
        for (const auto& listener : copy) if (listener) listener->stateChanged(event);
    }
}
