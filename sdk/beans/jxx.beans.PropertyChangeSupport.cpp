#include "beans/jxx.beans.PropertyChangeSupport.h"
#include <algorithm>
#include "beans/jxx.beans.PropertyChangeEvent.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
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
void PropertyChangeSupport::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) {
    output->writeObject(source_);
    std::vector<::jxx::Ptr<PropertyChangeListener>> serializableListeners;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& listener : listeners_) {
            if (std::dynamic_pointer_cast<::jxx::io::SerializableI>(listener) != nullptr) {
                serializableListeners.push_back(listener);
            }
        }
    }
    output->writeInt(static_cast<::jxx::lang::jint>(serializableListeners.size()));
    for (const auto& listener : serializableListeners) {
        output->writeObject(::jxx::CAST<::jxx::lang::Object>(listener));
    }
}

void PropertyChangeSupport::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) {
    source_ = input->readObject();
    listeners_.clear();
    const auto count = input->readInt();
    for (::jxx::lang::jint index = 0; index < count; ++index) {
        auto listener = ::jxx::CAST<PropertyChangeListener>(input->readObject());
        if (listener != nullptr) listeners_.push_back(listener);
    }
}

void PropertyChangeSupport::readObjectNoData() {
    source_ = nullptr;
    listeners_.clear();
}

} // namespace jxx::beans
