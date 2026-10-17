#include "beans/jxx.beans.PropertyChangeEvent.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::beans {

PropertyChangeEvent::PropertyChangeEvent(
    const ::jxx::Ptr<::jxx::lang::Object>& source,
    const ::jxx::Ptr<::jxx::lang::String>& propertyName,
    const ::jxx::Ptr<::jxx::lang::Object>& oldValue,
    const ::jxx::Ptr<::jxx::lang::Object>& newValue)
    : source_(source), propertyName_(propertyName),
      oldValue_(oldValue), newValue_(newValue) {
    if (source_ == nullptr || propertyName_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::Ptr<::jxx::lang::Object> PropertyChangeEvent::getSource() const { return source_; }
::jxx::Ptr<::jxx::lang::String> PropertyChangeEvent::getPropertyName() const { return propertyName_; }
::jxx::Ptr<::jxx::lang::Object> PropertyChangeEvent::getOldValue() const { return oldValue_; }
::jxx::Ptr<::jxx::lang::Object> PropertyChangeEvent::getNewValue() const { return newValue_; }

} // namespace jxx::beans
