#pragma once

#include "lang/management/jxx.lang.management.PlatformManagedObject.h"
#include "lang/jxx_types.h"

namespace jxx::lang::management {

class ClassLoadingMXBean
    : public ::jxx::lang::InterfaceBase<
          ClassLoadingMXBean, PlatformManagedObject> {
public:
    using Super = ::jxx::lang::InterfaceBase<
        ClassLoadingMXBean, PlatformManagedObject>;
    ~ClassLoadingMXBean() override = default;
    virtual ::jxx::lang::jlong getTotalLoadedClassCount() const = 0;
    virtual ::jxx::lang::jint getLoadedClassCount() const = 0;
    virtual ::jxx::lang::jlong getUnloadedClassCount() const = 0;
    virtual ::jxx::lang::jbool isVerbose() const = 0;
    virtual void setVerbose(::jxx::lang::jbool value) = 0;
};

} // namespace jxx::lang::management
