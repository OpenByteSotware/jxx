#pragma once

#include "lang/management/jxx.lang.management.LockInfo.h"
#include "lang/jxx.lang.StackTraceElement.h"

namespace jxx::lang::management {
class MonitorInfo final : public ::jxx::lang::ClassBase<MonitorInfo, LockInfo> {
public:
    using JxxSuper = LockInfo;
    using Super = ::jxx::lang::ClassBase<MonitorInfo, JxxSuper>;
    MonitorInfo(const ::jxx::Ptr<::jxx::lang::String>& className,
        ::jxx::lang::jint identityHashCode,
        ::jxx::lang::jint stackDepth,
        const ::jxx::Ptr<::jxx::lang::StackTraceElement>& stackFrame);
    ::jxx::lang::jint getLockedStackDepth() const;
    ::jxx::Ptr<::jxx::lang::StackTraceElement> getLockedStackFrame() const;
    static ::jxx::Ptr<MonitorInfo> from(
        const ::jxx::Ptr<::jxx::ext::management::openmbean::CompositeData>& data);
private:
    ::jxx::lang::jint stackDepth_;
    ::jxx::Ptr<::jxx::lang::StackTraceElement> stackFrame_;
};
} // namespace jxx::lang::management
