#pragma once

#include "util/jxx.util.EventObject.h"

namespace jxx::beans
{
    class PropertyChangeEvent : public ::jxx::lang::ClassBase<
        PropertyChangeEvent, ::jxx::util::EventObject>
    {
    public:
        using JxxSuper = ::jxx::util::EventObject;
        using Super = ::jxx::lang::ClassBase<PropertyChangeEvent, JxxSuper>;

        PropertyChangeEvent(
            const ::jxx::Ptr<::jxx::lang::Object>& source,
            const ::jxx::Ptr<::jxx::lang::String>& propertyName,
            const ::jxx::Ptr<::jxx::lang::Object>& oldValue,
            const ::jxx::Ptr<::jxx::lang::Object>& newValue);

        ::jxx::Ptr<::jxx::lang::String> getPropertyName() const;
        ::jxx::Ptr<::jxx::lang::Object> getOldValue() const;
        ::jxx::Ptr<::jxx::lang::Object> getNewValue() const;
        void setPropagationId(
            const ::jxx::Ptr<::jxx::lang::Object>& propagationId);
        ::jxx::Ptr<::jxx::lang::Object> getPropagationId() const;

    private:
        ::jxx::Ptr<::jxx::lang::String> propertyName_;
        ::jxx::Ptr<::jxx::lang::Object> oldValue_;
        ::jxx::Ptr<::jxx::lang::Object> newValue_;
        ::jxx::Ptr<::jxx::lang::Object> propagationId_;
    };
}
