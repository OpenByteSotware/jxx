#include "lang/management/jxx.lang.management.ClassInitialization.h"

#include <mutex>

#include "lang/management/jxx.lang.management.LockInfo.h"
#include "lang/management/jxx.lang.management.MemoryType.h"
#include "lang/management/jxx.lang.management.MemoryUsage.h"
#include "lang/management/jxx.lang.management.MonitorInfo.h"

namespace jxx::lang::management {
void initializeClasses() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)LockInfo::Class();
        (void)MemoryType::Class();
        (void)MemoryUsage::Class();
        (void)MonitorInfo::Class();
    });
}
void initialize() { initializeClasses(); }
} // namespace jxx::lang::management
