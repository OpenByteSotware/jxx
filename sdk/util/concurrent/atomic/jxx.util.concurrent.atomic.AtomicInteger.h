#pragma once

#include <atomic>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Number.h"

namespace jxx::lang { class String; }
namespace jxx::util::concurrent::atomic {

class AtomicInteger final : public ::jxx::lang::ClassBase<AtomicInteger, ::jxx::lang::Number> {
public:
    using JxxSuper = ::jxx::lang::Number;
    using Super = ::jxx::lang::ClassBase<AtomicInteger, JxxSuper>;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<AtomicInteger, JxxSuper>;
    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
    AtomicInteger();
    explicit AtomicInteger(::jxx::lang::jint initialValue);
    ::jxx::lang::jint get() const noexcept;
    void set(::jxx::lang::jint value) noexcept;
    void lazySet(::jxx::lang::jint value) noexcept;
    ::jxx::lang::jint getAndSet(::jxx::lang::jint value) noexcept;
    ::jxx::lang::jbool compareAndSet(::jxx::lang::jint expected, ::jxx::lang::jint update) noexcept;
    ::jxx::lang::jbool weakCompareAndSet(::jxx::lang::jint expected, ::jxx::lang::jint update) noexcept;
    ::jxx::lang::jint getAndIncrement() noexcept;
    ::jxx::lang::jint getAndDecrement() noexcept;
    ::jxx::lang::jint getAndAdd(::jxx::lang::jint delta) noexcept;
    ::jxx::lang::jint incrementAndGet() noexcept;
    ::jxx::lang::jint decrementAndGet() noexcept;
    ::jxx::lang::jint addAndGet(::jxx::lang::jint delta) noexcept;
    ::jxx::lang::jint intValue() const override;
    ::jxx::lang::jlong longValue() const override;
    ::jxx::lang::jfloat floatValue() const override;
    ::jxx::lang::jdouble doubleValue() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
private:
    std::atomic<::jxx::lang::jint> value_;
};
} // namespace jxx::util::concurrent::atomic
