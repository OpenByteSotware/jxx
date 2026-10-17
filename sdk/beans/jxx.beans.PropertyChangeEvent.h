#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::beans {

class PropertyChangeEvent final
    : public ::jxx::lang::ClassBase<
          PropertyChangeEvent,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<PropertyChangeEvent, JxxSuper>;

    PropertyChangeEvent(
        const ::jxx::Ptr<::jxx::lang::Object>& source,
        const ::jxx::Ptr<::jxx::lang::String>& propertyName,
        const ::jxx::Ptr<::jxx::lang::Object>& oldValue,
        const ::jxx::Ptr<::jxx::lang::Object>& newValue);

    ::jxx::Ptr<::jxx::lang::Object> getSource() const;
    ::jxx::Ptr<::jxx::lang::String> getPropertyName() const;
    ::jxx::Ptr<::jxx::lang::Object> getOldValue() const;
    ::jxx::Ptr<::jxx::lang::Object> getNewValue() const;

private:
    ::jxx::Ptr<::jxx::lang::Object> source_;
    ::jxx::Ptr<::jxx::lang::String> propertyName_;
    ::jxx::Ptr<::jxx::lang::Object> oldValue_;
    ::jxx::Ptr<::jxx::lang::Object> newValue_;
};

} // namespace jxx::beans
