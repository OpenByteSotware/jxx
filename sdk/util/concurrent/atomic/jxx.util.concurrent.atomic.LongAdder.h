#pragma once
#include <atomic>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Number.h"
namespace jxx::lang { class String; }
namespace jxx::util::concurrent::atomic {
class LongAdder final : public ::jxx::lang::ClassBase<LongAdder,::jxx::lang::Number,::jxx::io::SerializableI> {
public:
 using JxxSuper=::jxx::lang::Number; using Super=::jxx::lang::ClassBase<LongAdder,JxxSuper,::jxx::io::SerializableI>; using JxxClassInfoMarker=::jxx::lang::ClassInfo<LongAdder,JxxSuper,::jxx::io::SerializableI>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class(); LongAdder();
 void add(::jxx::lang::jlong x) noexcept; void increment() noexcept; void decrement() noexcept;
 ::jxx::lang::jlong sum() const noexcept; void reset() noexcept; ::jxx::lang::jlong sumThenReset() noexcept;
 ::jxx::lang::jint intValue() const override; ::jxx::lang::jlong longValue() const override; ::jxx::lang::jfloat floatValue() const override; ::jxx::lang::jdouble doubleValue() const override; ::jxx::Ptr<::jxx::lang::String> toString() const override;
 void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&) override; void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&) override; void readObjectNoData() override;
private: std::atomic<::jxx::lang::jlong> value_{0};
}; }
