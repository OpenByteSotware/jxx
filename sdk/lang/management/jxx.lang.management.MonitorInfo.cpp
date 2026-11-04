#include "lang/management/jxx.lang.management.MonitorInfo.h"

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"

namespace jxx::lang::management {
MonitorInfo::MonitorInfo(const ::jxx::Ptr<::jxx::lang::String>& className,
    ::jxx::lang::jint identityHashCode, ::jxx::lang::jint stackDepth,
    const ::jxx::Ptr<::jxx::lang::StackTraceElement>& stackFrame)
    : Super(className, identityHashCode), stackDepth_(stackDepth),
      stackFrame_(stackFrame) {
    if ((stackDepth >= 0 && stackFrame == nullptr) ||
        (stackDepth < 0 && stackFrame != nullptr)) {
        throw ::jxx::lang::IllegalArgumentException(
            "stackDepth and stackFrame are inconsistent");
    }
}
::jxx::lang::jint MonitorInfo::getLockedStackDepth() const { return stackDepth_; }
::jxx::Ptr<::jxx::lang::StackTraceElement>
MonitorInfo::getLockedStackFrame() const { return stackFrame_; }
::jxx::Ptr<MonitorInfo> MonitorInfo::from(
    const ::jxx::Ptr<::jxx::ext::management::openmbean::CompositeData>& data) {
    if (data == nullptr) return nullptr;
    throw ::jxx::lang::UnsupportedOperationException(
        "CompositeData requires javax.management.openmbean support");
}
} // namespace jxx::lang::management
