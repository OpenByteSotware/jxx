#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/management/jxx.lang.management.MemoryUsage.h"

namespace jxx::lang::management {

class MemoryNotificationInfo final
    : public ::jxx::lang::ClassBase<MemoryNotificationInfo, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<MemoryNotificationInfo, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::String> MEMORY_THRESHOLD_EXCEEDED;
    static ::jxx::Ptr<::jxx::lang::String>
        MEMORY_COLLECTION_THRESHOLD_EXCEEDED;

    MemoryNotificationInfo(
        const ::jxx::Ptr<::jxx::lang::String>& poolName,
        const ::jxx::Ptr<MemoryUsage>& usage,
        ::jxx::lang::jlong count);
    ::jxx::Ptr<::jxx::lang::String> getPoolName() const;
    ::jxx::Ptr<MemoryUsage> getUsage() const;
    ::jxx::lang::jlong getCount() const;
    static ::jxx::Ptr<MemoryNotificationInfo> from(
        const ::jxx::Ptr<::jxx::ext::management::openmbean::CompositeData>& data);
private:
    ::jxx::Ptr<::jxx::lang::String> poolName_;
    ::jxx::Ptr<MemoryUsage> usage_;
    ::jxx::lang::jlong count_;
};

} // namespace jxx::lang::management
