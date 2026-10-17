#pragma once
#include <mutex>
#include <vector>
#include "beans/jxx.beans.PropertyChangeListener.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::beans {
class PropertyChangeSupport final
    : public ::jxx::lang::ClassBase<PropertyChangeSupport, ::jxx::lang::Object> {
public:
    explicit PropertyChangeSupport(
        const ::jxx::Ptr<::jxx::lang::Object>& source);
    void addPropertyChangeListener(
        const ::jxx::Ptr<PropertyChangeListener>& listener);
    void removePropertyChangeListener(
        const ::jxx::Ptr<PropertyChangeListener>& listener);
    void firePropertyChange(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& oldValue,
        const ::jxx::Ptr<::jxx::lang::Object>& newValue);
private:
    ::jxx::Ptr<::jxx::lang::Object> source_;
    std::mutex mutex_;
    std::vector<::jxx::Ptr<PropertyChangeListener>> listeners_;
};
} // namespace jxx::beans
