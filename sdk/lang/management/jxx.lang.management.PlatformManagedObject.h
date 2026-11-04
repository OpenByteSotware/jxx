#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::ext::management { class ObjectName; }

namespace jxx::lang::management {

class PlatformManagedObject
    : public ::jxx::lang::InterfaceBase<PlatformManagedObject> {
public:
    using Super = ::jxx::lang::InterfaceBase<PlatformManagedObject>;
    ~PlatformManagedObject() override = default;
    virtual ::jxx::Ptr<::jxx::ext::management::ObjectName>
        getObjectName() const = 0;
};

} // namespace jxx::lang::management
