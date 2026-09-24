#pragma once

#include <atomic>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Number.h"

namespace jxx::lang { class String; }
namespace jxx::util::concurrent::atomic {

class AtomicLong final : public ::jxx::lang::ClassBase<AtomicLong, ::jxx::lang::Number> {
public:
    using JxxSuper = ::jxx::lang::Number;
    using Super = ::jxx::lang::ClassBase<AtomicLong, JxxSuper>;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<AtomicLong, JxxSuper>;
    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
    AtomicLong();
    explicit AtomicLong(::jxx::lang::jlong initialValue);
    ::jxx::lang::jlong get() const noexcept;
    void set(::jxx::lang::jlong value) noexcept;
    void lazySet(::jxx::lang::jlong value) noexcept;
    ::jxx::lang::jlong getAndSet(::jxx::lang::jlong value) noexcept;
    ::jxx::lang::jbool compareAndSet(::jxx::lang::jlong expected, ::jxx::lang::jlong update) noexcept;
    ::jxx::lang::jbool weakCompareAndSet(::jxx::lang::jlong expected, ::jxx::lang::jlong update) noexcept;
    ::jxx::lang::jlong getAndIncrement() noexcept;
    ::jxx::lang::jlong getAndDecrement() noexcept;
    ::jxx::lang::jlong getAndAdd(::jxx::lang::jlong delta) noexcept;
    ::jxx::lang::jlong incrementAndGet() noexcept;
    ::jxx::lang::jlong decrementAndGet() noexcept;
    ::jxx::lang::jlong addAndGet(::jxx::lang::jlong delta) noexcept;
    ::jxx::lang::jint intValue() const override;
    ::jxx::lang::jlong longValue() const override;
    ::jxx::lang::jfloat floatValue() const override;
    ::jxx::lang::jdouble doubleValue() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
private:
    std::atomic<::jxx::lang::jlong> value_;
};
} // namespace jxx::util::concurrent::atomic
