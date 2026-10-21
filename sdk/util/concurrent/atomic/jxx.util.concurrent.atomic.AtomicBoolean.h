#pragma once

#include <atomic>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::io { class ObjectInputStream; class ObjectOutputStream; }
namespace jxx::lang { class String; }

namespace jxx::util::concurrent::atomic {

class AtomicBoolean final
    : public ::jxx::lang::ClassBase<AtomicBoolean, ::jxx::lang::Object, ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<AtomicBoolean, JxxSuper, ::jxx::io::SerializableI>;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<AtomicBoolean, JxxSuper, ::jxx::io::SerializableI>;

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

    void writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) override;
    void readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) override;
    void readObjectNoData() override;

private:
    std::atomic<::jxx::lang::jbool> value_;
};

} // namespace jxx::util::concurrent::atomic
