#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicInteger.h"

#include <string>
#include "lang/jxx.lang.String.h"
namespace jxx::util::concurrent::atomic {
::jxx::Ptr<::jxx::lang::ClassAny> AtomicInteger::Class() { return JxxClassInfoMarker::Class(); }
AtomicInteger::AtomicInteger() : Super(), value_(0) {}
AtomicInteger::AtomicInteger(::jxx::lang::jint v) : Super(), value_(v) {}
::jxx::lang::jint AtomicInteger::get() const noexcept { return value_.load(std::memory_order_acquire); }
void AtomicInteger::set(::jxx::lang::jint v) noexcept { value_.store(v,std::memory_order_release); }
void AtomicInteger::lazySet(::jxx::lang::jint v) noexcept { value_.store(v,std::memory_order_relaxed); }
::jxx::lang::jint AtomicInteger::getAndSet(::jxx::lang::jint v) noexcept { return value_.exchange(v); }
::jxx::lang::jbool AtomicInteger::compareAndSet(::jxx::lang::jint e,::jxx::lang::jint u) noexcept { return value_.compare_exchange_strong(e,u); }
::jxx::lang::jbool AtomicInteger::weakCompareAndSet(::jxx::lang::jint e,::jxx::lang::jint u) noexcept { return value_.compare_exchange_weak(e,u); }
::jxx::lang::jint AtomicInteger::getAndIncrement() noexcept { return value_.fetch_add(1); }
::jxx::lang::jint AtomicInteger::getAndDecrement() noexcept { return value_.fetch_sub(1); }
::jxx::lang::jint AtomicInteger::getAndAdd(::jxx::lang::jint d) noexcept { return value_.fetch_add(d); }
::jxx::lang::jint AtomicInteger::incrementAndGet() noexcept { return value_.fetch_add(1)+1; }
::jxx::lang::jint AtomicInteger::decrementAndGet() noexcept { return value_.fetch_sub(1)-1; }
::jxx::lang::jint AtomicInteger::addAndGet(::jxx::lang::jint d) noexcept { return value_.fetch_add(d)+d; }
::jxx::lang::jint AtomicInteger::intValue() const { return static_cast<::jxx::lang::jint>(get()); }
::jxx::lang::jlong AtomicInteger::longValue() const { return static_cast<::jxx::lang::jlong>(get()); }
::jxx::lang::jfloat AtomicInteger::floatValue() const { return static_cast<::jxx::lang::jfloat>(get()); }
::jxx::lang::jdouble AtomicInteger::doubleValue() const { return static_cast<::jxx::lang::jdouble>(get()); }
::jxx::Ptr<::jxx::lang::String> AtomicInteger::toString() const { return ::jxx::NEW<::jxx::lang::String>(std::to_string(get())); }
} // namespace jxx::util::concurrent::atomic
