#pragma once

#include "lang/management/jxx.lang.management.PlatformManagedObject.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang::management {

class CompilationMXBean
    : public ::jxx::lang::InterfaceBase<
          CompilationMXBean, PlatformManagedObject> {
public:
    using Super = ::jxx::lang::InterfaceBase<
        CompilationMXBean, PlatformManagedObject>;
    ~CompilationMXBean() override = default;
    virtual ::jxx::Ptr<::jxx::lang::String> getName() const = 0;
    virtual ::jxx::lang::jbool
        isCompilationTimeMonitoringSupported() const = 0;
    virtual ::jxx::lang::jlong getTotalCompilationTime() const = 0;
};

} // namespace jxx::lang::management
