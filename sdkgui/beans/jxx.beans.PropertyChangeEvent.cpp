#include "beans/jxx.beans.PropertyChangeEvent.h"

namespace jxx::beans
{
    PropertyChangeEvent::PropertyChangeEvent(
        const ::jxx::Ptr<::jxx::lang::Object>& source,
        const ::jxx::Ptr<::jxx::lang::String>& propertyName,
        const ::jxx::Ptr<::jxx::lang::Object>& oldValue,
        const ::jxx::Ptr<::jxx::lang::Object>& newValue)
        : Super(source),
          propertyName_(propertyName),
          oldValue_(oldValue),
          newValue_(newValue)
    {
    }

    ::jxx::Ptr<::jxx::lang::String>
    PropertyChangeEvent::getPropertyName() const
    {
        return propertyName_;
    }

    ::jxx::Ptr<::jxx::lang::Object>
    PropertyChangeEvent::getOldValue() const
    {
        return oldValue_;
    }

    ::jxx::Ptr<::jxx::lang::Object>
    PropertyChangeEvent::getNewValue() const
    {
        return newValue_;
    }

    void PropertyChangeEvent::setPropagationId(
        const ::jxx::Ptr<::jxx::lang::Object>& propagationId)
    {
        propagationId_ = propagationId;
    }

    ::jxx::Ptr<::jxx::lang::Object>
    PropertyChangeEvent::getPropagationId() const
    {
        return propagationId_;
    }
}
