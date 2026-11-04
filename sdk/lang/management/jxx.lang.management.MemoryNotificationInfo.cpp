#include "lang/management/jxx.lang.management.MemoryNotificationInfo.h"

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"

namespace jxx::lang::management {
::jxx::Ptr<::jxx::lang::String> MemoryNotificationInfo::MEMORY_THRESHOLD_EXCEEDED =
    ::jxx::NEW<::jxx::lang::String>(
        "java.management.memory.threshold.exceeded");
::jxx::Ptr<::jxx::lang::String>
MemoryNotificationInfo::MEMORY_COLLECTION_THRESHOLD_EXCEEDED =
    ::jxx::NEW<::jxx::lang::String>(
        "java.management.memory.collection.threshold.exceeded");
MemoryNotificationInfo::MemoryNotificationInfo(
    const ::jxx::Ptr<::jxx::lang::String>& poolName,
    const ::jxx::Ptr<MemoryUsage>& usage, ::jxx::lang::jlong count)
    : poolName_(poolName), usage_(usage), count_(count) {
    if (poolName_ == nullptr) throw ::jxx::lang::NullPointerException("poolName");
    if (usage_ == nullptr) throw ::jxx::lang::NullPointerException("usage");
}
::jxx::Ptr<::jxx::lang::String> MemoryNotificationInfo::getPoolName() const {
    return poolName_;
}
::jxx::Ptr<MemoryUsage> MemoryNotificationInfo::getUsage() const { return usage_; }
::jxx::lang::jlong MemoryNotificationInfo::getCount() const { return count_; }
::jxx::Ptr<MemoryNotificationInfo> MemoryNotificationInfo::from(
    const ::jxx::Ptr<::jxx::ext::management::openmbean::CompositeData>& data) {
    if (data == nullptr) return nullptr;
    throw ::jxx::lang::UnsupportedOperationException(
        "CompositeData requires javax.management.openmbean support");
}
} // namespace jxx::lang::management
