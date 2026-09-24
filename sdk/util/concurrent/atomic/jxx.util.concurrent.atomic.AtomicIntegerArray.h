#pragma once

#include <mutex>
#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::util::concurrent::atomic {
class AtomicIntegerArray final : public ::jxx::lang::ClassBase<AtomicIntegerArray,::jxx::lang::Object,::jxx::io::SerializableI> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<AtomicIntegerArray,JxxSuper,::jxx::io::SerializableI>; using JxxClassInfoMarker=::jxx::lang::ClassInfo<AtomicIntegerArray,JxxSuper,::jxx::io::SerializableI>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
 explicit AtomicIntegerArray(::jxx::lang::jint length);
 explicit AtomicIntegerArray(const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint,1U>>& array);
 ::jxx::lang::jint length() const noexcept;
 ::jxx::lang::jint get(::jxx::lang::jint index) const;
 void set(::jxx::lang::jint index,::jxx::lang::jint value);
 void lazySet(::jxx::lang::jint index,::jxx::lang::jint value);
 ::jxx::lang::jint getAndSet(::jxx::lang::jint index,::jxx::lang::jint value);
 ::jxx::lang::jbool compareAndSet(::jxx::lang::jint index,::jxx::lang::jint expected,::jxx::lang::jint update);
 ::jxx::lang::jbool weakCompareAndSet(::jxx::lang::jint index,::jxx::lang::jint expected,::jxx::lang::jint update);
 ::jxx::lang::jint getAndIncrement(::jxx::lang::jint index);
 ::jxx::lang::jint getAndDecrement(::jxx::lang::jint index);
 ::jxx::lang::jint getAndAdd(::jxx::lang::jint index,::jxx::lang::jint delta);
 ::jxx::lang::jint incrementAndGet(::jxx::lang::jint index);
 ::jxx::lang::jint decrementAndGet(::jxx::lang::jint index);
 ::jxx::lang::jint addAndGet(::jxx::lang::jint index,::jxx::lang::jint delta);
 void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&) override;
 void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&) override;
 void readObjectNoData() override;
private:
 static std::size_t checkedLength_(::jxx::lang::jint length);
 std::size_t index_(::jxx::lang::jint index) const;
 mutable std::mutex mutex_; std::vector<::jxx::lang::jint> values_;
};
} // namespace jxx::util::concurrent::atomic
