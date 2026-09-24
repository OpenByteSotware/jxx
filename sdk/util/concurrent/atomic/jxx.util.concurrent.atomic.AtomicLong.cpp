#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicLong.h"

#include <string>
#include "lang/jxx.lang.String.h"
namespace jxx::util::concurrent::atomic {
::jxx::Ptr<::jxx::lang::ClassAny> AtomicLong::Class() { return JxxClassInfoMarker::Class(); }
AtomicLong::AtomicLong() : Super(), value_(0) {}
AtomicLong::AtomicLong(::jxx::lang::jlong v) : Super(), value_(v) {}
::jxx::lang::jlong AtomicLong::get() const noexcept { return value_.load(std::memory_order_acquire); }
void AtomicLong::set(::jxx::lang::jlong v) noexcept { value_.store(v,std::memory_order_release); }
void AtomicLong::lazySet(::jxx::lang::jlong v) noexcept { value_.store(v,std::memory_order_relaxed); }
::jxx::lang::jlong AtomicLong::getAndSet(::jxx::lang::jlong v) noexcept { return value_.exchange(v); }
::jxx::lang::jbool AtomicLong::compareAndSet(::jxx::lang::jlong e,::jxx::lang::jlong u) noexcept { return value_.compare_exchange_strong(e,u); }
::jxx::lang::jbool AtomicLong::weakCompareAndSet(::jxx::lang::jlong e,::jxx::lang::jlong u) noexcept { return value_.compare_exchange_weak(e,u); }
::jxx::lang::jlong AtomicLong::getAndIncrement() noexcept { return value_.fetch_add(1); }
::jxx::lang::jlong AtomicLong::getAndDecrement() noexcept { return value_.fetch_sub(1); }
::jxx::lang::jlong AtomicLong::getAndAdd(::jxx::lang::jlong d) noexcept { return value_.fetch_add(d); }
::jxx::lang::jlong AtomicLong::incrementAndGet() noexcept { return value_.fetch_add(1)+1; }
::jxx::lang::jlong AtomicLong::decrementAndGet() noexcept { return value_.fetch_sub(1)-1; }
::jxx::lang::jlong AtomicLong::addAndGet(::jxx::lang::jlong d) noexcept { return value_.fetch_add(d)+d; }
::jxx::lang::jint AtomicLong::intValue() const { return static_cast<::jxx::lang::jint>(get()); }
::jxx::lang::jlong AtomicLong::longValue() const { return static_cast<::jxx::lang::jlong>(get()); }
::jxx::lang::jfloat AtomicLong::floatValue() const { return static_cast<::jxx::lang::jfloat>(get()); }
::jxx::lang::jdouble AtomicLong::doubleValue() const { return static_cast<::jxx::lang::jdouble>(get()); }
::jxx::Ptr<::jxx::lang::String> AtomicLong::toString() const { return ::jxx::NEW<::jxx::lang::String>(std::to_string(get())); }
} // namespace jxx::util::concurrent::atomic
