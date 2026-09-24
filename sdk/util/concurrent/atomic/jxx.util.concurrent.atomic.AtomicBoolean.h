#pragma once

#include <atomic>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang { class String; }

namespace jxx::util::concurrent::atomic {

class AtomicBoolean final
    : public ::jxx::lang::ClassBase<AtomicBoolean, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<AtomicBoolean, JxxSuper>;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<AtomicBoolean, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    AtomicBoolean();
    explicit AtomicBoolean(::jxx::lang::jbool initialValue);

    ::jxx::lang::jbool get() const noexcept;
    void set(::jxx::lang::jbool value) noexcept;
    void lazySet(::jxx::lang::jbool value) noexcept;
    ::jxx::lang::jbool getAndSet(::jxx::lang::jbool value) noexcept;
    ::jxx::lang::jbool compareAndSet(
        ::jxx::lang::jbool expected,
        ::jxx::lang::jbool update) noexcept;
    ::jxx::lang::jbool weakCompareAndSet(
        ::jxx::lang::jbool expected,
        ::jxx::lang::jbool update) noexcept;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

private:
    std::atomic<::jxx::lang::jbool> value_;
};

} // namespace jxx::util::concurrent::atomic
