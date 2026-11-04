#include "lang/management/jxx.lang.management.MemoryUsage.h"

#include <sstream>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"

namespace jxx::lang::management {
MemoryUsage::MemoryUsage(::jxx::lang::jlong init, ::jxx::lang::jlong used,
    ::jxx::lang::jlong committed, ::jxx::lang::jlong max)
    : init_(init), used_(used), committed_(committed), max_(max) {
    if (init < -1 || max < -1 || used < 0 || committed < 0 ||
        used > committed || (max >= 0 && committed > max)) {
        throw ::jxx::lang::IllegalArgumentException("Invalid memory usage");
    }
}
::jxx::lang::jlong MemoryUsage::getInit() const { return init_; }
::jxx::lang::jlong MemoryUsage::getUsed() const { return used_; }
::jxx::lang::jlong MemoryUsage::getCommitted() const { return committed_; }
::jxx::lang::jlong MemoryUsage::getMax() const { return max_; }
::jxx::Ptr<::jxx::lang::String> MemoryUsage::toString() const {
    std::ostringstream out;
    out << "init = " << init_ << "(" << (init_ >> 10) << "K) used = "
        << used_ << "(" << (used_ >> 10) << "K) committed = " << committed_
        << "(" << (committed_ >> 10) << "K) max = " << max_ << "("
        << (max_ >> 10) << "K)";
    return ::jxx::NEW<::jxx::lang::String>(out.str());
}
::jxx::Ptr<MemoryUsage> MemoryUsage::from(
    const ::jxx::Ptr<::jxx::ext::management::openmbean::CompositeData>& data) {
    if (data == nullptr) return nullptr;
    throw ::jxx::lang::UnsupportedOperationException(
        "CompositeData requires javax.management.openmbean support");
}
} // namespace jxx::lang::management
