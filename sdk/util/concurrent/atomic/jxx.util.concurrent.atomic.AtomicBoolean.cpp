#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicBoolean.h"

#include "lang/jxx.lang.String.h"

namespace jxx::util::concurrent::atomic {

::jxx::Ptr<::jxx::lang::ClassAny> AtomicBoolean::Class() {
    return JxxClassInfoMarker::Class();
}
AtomicBoolean::AtomicBoolean() : Super(), value_(false) {}
AtomicBoolean::AtomicBoolean(::jxx::lang::jbool initialValue)
    : Super(), value_(initialValue) {}
::jxx::lang::jbool AtomicBoolean::get() const noexcept {
    return value_.load(std::memory_order_acquire);
}
void AtomicBoolean::set(::jxx::lang::jbool value) noexcept {
    value_.store(value, std::memory_order_release);
}
void AtomicBoolean::lazySet(::jxx::lang::jbool value) noexcept {
    value_.store(value, std::memory_order_relaxed);
}
::jxx::lang::jbool AtomicBoolean::getAndSet(::jxx::lang::jbool value) noexcept {
    return value_.exchange(value, std::memory_order_acq_rel);
}
::jxx::lang::jbool AtomicBoolean::compareAndSet(
    ::jxx::lang::jbool expected, ::jxx::lang::jbool update) noexcept {
    return value_.compare_exchange_strong(
        expected, update, std::memory_order_acq_rel);
}
::jxx::lang::jbool AtomicBoolean::weakCompareAndSet(
    ::jxx::lang::jbool expected, ::jxx::lang::jbool update) noexcept {
    return value_.compare_exchange_weak(
        expected, update, std::memory_order_acq_rel);
}
::jxx::Ptr<::jxx::lang::String> AtomicBoolean::toString() const {
    return ::jxx::NEW<::jxx::lang::String>(get() ? "true" : "false");
}

} // namespace jxx::util::concurrent::atomic
