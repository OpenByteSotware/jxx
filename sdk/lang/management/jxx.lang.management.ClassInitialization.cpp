#include "lang/management/jxx.lang.management.ClassInitialization.h"

#include <mutex>

#include "lang/management/jxx.lang.management.LockInfo.h"
#include "lang/management/jxx.lang.management.ManagementPermission.h"
#include "lang/management/jxx.lang.management.MemoryNotificationInfo.h"
#include "lang/management/jxx.lang.management.ThreadInfo.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/management/jxx.lang.management.MemoryType.h"
#include "lang/management/jxx.lang.management.MemoryUsage.h"
#include "lang/management/jxx.lang.management.MonitorInfo.h"

namespace jxx::lang::management {
void initializeClasses() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)LockInfo::Class();
        (void)ManagementPermission::Class();
        ::jxx::io::serialization::registerSerializableClass<ManagementPermission>(
            static_cast<::jxx::lang::jlong>(1897496590799378737LL),
            [] { return ::jxx::NEW<ManagementPermission>(
                ::jxx::NEW<::jxx::lang::String>("monitor")); });
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            ManagementPermission, 1U, 2U>();
        (void)MemoryNotificationInfo::Class();
        (void)ThreadInfo::Class();
        (void)MemoryType::Class();
        (void)MemoryUsage::Class();
        (void)MonitorInfo::Class();
    });
}
void initialize() { initializeClasses(); }
} // namespace jxx::lang::management
