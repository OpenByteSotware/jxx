#include "beans/jxx.beans.PropertyChangeSupport.h"
#include <algorithm>
#include "beans/jxx.beans.PropertyChangeEvent.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::beans {
PropertyChangeSupport::PropertyChangeSupport(
    const ::jxx::Ptr<::jxx::lang::Object>& source) : source_(source) {
    if (source_ == nullptr) throw ::jxx::lang::NullPointerException();
}
void PropertyChangeSupport::addPropertyChangeListener(
    const ::jxx::Ptr<PropertyChangeListener>& listener) {
    if (listener == nullptr) return;
    std::lock_guard<std::mutex> lock(mutex_);
    listeners_.push_back(listener);
}
void PropertyChangeSupport::removePropertyChangeListener(
    const ::jxx::Ptr<PropertyChangeListener>& listener) {
    std::lock_guard<std::mutex> lock(mutex_);
    listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), listener), listeners_.end());
}
void PropertyChangeSupport::firePropertyChange(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::Object>& oldValue,
    const ::jxx::Ptr<::jxx::lang::Object>& newValue) {
    std::vector<::jxx::Ptr<PropertyChangeListener>> snapshot;
    { std::lock_guard<std::mutex> lock(mutex_); snapshot = listeners_; }
    const auto event = ::jxx::NEW<PropertyChangeEvent>(source_, name, oldValue, newValue);
    for (const auto& listener : snapshot) if (listener != nullptr) listener->propertyChange(event);
}
} // namespace jxx::beans
