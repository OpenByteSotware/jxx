#pragma once

#include "lang/management/jxx.lang.management.PlatformManagedObject.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang::management {

class OperatingSystemMXBean
    : public ::jxx::lang::InterfaceBase<
          OperatingSystemMXBean, PlatformManagedObject> {
public:
    using Super = ::jxx::lang::InterfaceBase<
        OperatingSystemMXBean, PlatformManagedObject>;
    ~OperatingSystemMXBean() override = default;
    virtual ::jxx::Ptr<::jxx::lang::String> getName() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getArch() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getVersion() const = 0;
    virtual ::jxx::lang::jint getAvailableProcessors() const = 0;
    virtual ::jxx::lang::jdouble getSystemLoadAverage() const = 0;
};

} // namespace jxx::lang::management
