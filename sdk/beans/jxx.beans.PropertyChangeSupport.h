#pragma once
#include <mutex>
#include <vector>
#include "beans/jxx.beans.PropertyChangeListener.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "io/jxx.io.SerializableI.h"
namespace jxx::beans {
class PropertyChangeSupport
    : public ::jxx::lang::ClassBase<PropertyChangeSupport, ::jxx::lang::Object, ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<PropertyChangeSupport, JxxSuper, ::jxx::io::SerializableI>;

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
    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) override;
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) override;
    void readObjectNoData() override;
private:
    ::jxx::Ptr<::jxx::lang::Object> source_;
    std::mutex mutex_;
    std::vector<::jxx::Ptr<PropertyChangeListener>> listeners_;
};
} // namespace jxx::beans
