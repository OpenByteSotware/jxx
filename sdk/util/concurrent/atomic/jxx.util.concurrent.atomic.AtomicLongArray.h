#pragma once

#include <mutex>
#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::util::concurrent::atomic {
class AtomicLongArray final : public ::jxx::lang::ClassBase<AtomicLongArray,::jxx::lang::Object,::jxx::io::SerializableI> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<AtomicLongArray,JxxSuper,::jxx::io::SerializableI>; using JxxClassInfoMarker=::jxx::lang::ClassInfo<AtomicLongArray,JxxSuper,::jxx::io::SerializableI>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
 explicit AtomicLongArray(::jxx::lang::jint length);
 explicit AtomicLongArray(const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jlong,1U>>& array);
 ::jxx::lang::jint length() const noexcept;
 ::jxx::lang::jlong get(::jxx::lang::jint index) const;
 void set(::jxx::lang::jint index,::jxx::lang::jlong value);
 void lazySet(::jxx::lang::jint index,::jxx::lang::jlong value);
 ::jxx::lang::jlong getAndSet(::jxx::lang::jint index,::jxx::lang::jlong value);
 ::jxx::lang::jbool compareAndSet(::jxx::lang::jint index,::jxx::lang::jlong expected,::jxx::lang::jlong update);
 ::jxx::lang::jbool weakCompareAndSet(::jxx::lang::jint index,::jxx::lang::jlong expected,::jxx::lang::jlong update);
 ::jxx::lang::jlong getAndIncrement(::jxx::lang::jint index);
 ::jxx::lang::jlong getAndDecrement(::jxx::lang::jint index);
 ::jxx::lang::jlong getAndAdd(::jxx::lang::jint index,::jxx::lang::jlong delta);
 ::jxx::lang::jlong incrementAndGet(::jxx::lang::jint index);
 ::jxx::lang::jlong decrementAndGet(::jxx::lang::jint index);
 ::jxx::lang::jlong addAndGet(::jxx::lang::jint index,::jxx::lang::jlong delta);
 void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&) override;
 void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&) override;
 void readObjectNoData() override;
private:
 static std::size_t checkedLength_(::jxx::lang::jint length);
 std::size_t index_(::jxx::lang::jint index) const;
 mutable std::mutex mutex_; std::vector<::jxx::lang::jlong> values_;
};
} // namespace jxx::util::concurrent::atomic
