#include "beans/jxx.beans.PropertyChangeEvent.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"

namespace jxx::beans {

PropertyChangeEvent::PropertyChangeEvent(
    const ::jxx::Ptr<::jxx::lang::Object>& source,
    const ::jxx::Ptr<::jxx::lang::String>& propertyName,
    const ::jxx::Ptr<::jxx::lang::Object>& oldValue,
    const ::jxx::Ptr<::jxx::lang::Object>& newValue)
    : Super(source), propertyName_(propertyName),
      oldValue_(oldValue), newValue_(newValue) {
    if (propertyName_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::Ptr<::jxx::lang::String> PropertyChangeEvent::getPropertyName() const { return propertyName_; }
::jxx::Ptr<::jxx::lang::Object> PropertyChangeEvent::getOldValue() const { return oldValue_; }
::jxx::Ptr<::jxx::lang::Object> PropertyChangeEvent::getNewValue() const { return newValue_; }

void PropertyChangeEvent::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) {
    JxxSuper::writeObject(output);
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(propertyName_));
    output->writeObject(oldValue_);
    output->writeObject(newValue_);
}

void PropertyChangeEvent::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) {
    JxxSuper::readObject(input);
    propertyName_ = ::jxx::CAST<::jxx::lang::String>(input->readObject());
    oldValue_ = input->readObject();
    newValue_ = input->readObject();
}

void PropertyChangeEvent::readObjectNoData() {
    JxxSuper::readObjectNoData();
    propertyName_ = nullptr;
    oldValue_ = nullptr;
    newValue_ = nullptr;
}

} // namespace jxx::beans
