#include "util/concurrent/atomic/jxx.util.concurrent.atomic.ClassInitialization.h"

#include <mutex>

#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"

#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicBoolean.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicInteger.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicIntegerArray.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicLong.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicLongArray.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.DoubleAccumulator.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.DoubleAdder.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.LongAccumulator.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.LongAdder.h"

namespace jxx::util::concurrent::atomic {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<AtomicBoolean, ::jxx::lang::Object>::Class();
        (void)::jxx::lang::ClassBase<AtomicInteger, ::jxx::lang::Number>::Class();
        (void)::jxx::lang::ClassBase<AtomicIntegerArray, ::jxx::lang::Object, ::jxx::io::SerializableI>::Class();
        ::jxx::io::serialization::registerSerializableClass<AtomicIntegerArray>(
            1,
            [] { return ::jxx::NEW<AtomicIntegerArray>(0); });
        (void)::jxx::lang::ClassBase<AtomicLong, ::jxx::lang::Number>::Class();
        (void)::jxx::lang::ClassBase<AtomicLongArray, ::jxx::lang::Object, ::jxx::io::SerializableI>::Class();
        ::jxx::io::serialization::registerSerializableClass<AtomicLongArray>(
            1,
            [] { return ::jxx::NEW<AtomicLongArray>(0); });
        (void)::jxx::lang::ClassBase<DoubleAccumulator, ::jxx::lang::Number, ::jxx::io::SerializableI>::Class();
        (void)::jxx::lang::ClassBase<DoubleAdder, ::jxx::lang::Number, ::jxx::io::SerializableI>::Class();
        ::jxx::io::serialization::registerSerializableClass<DoubleAdder>(1);
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            DoubleAdder, 1U, 2U>();
        (void)::jxx::lang::ClassBase<LongAccumulator, ::jxx::lang::Number, ::jxx::io::SerializableI>::Class();
        (void)::jxx::lang::ClassBase<LongAdder, ::jxx::lang::Number, ::jxx::io::SerializableI>::Class();
        ::jxx::io::serialization::registerSerializableClass<LongAdder>(1);
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            LongAdder, 1U, 2U>();
    });
}

} // namespace jxx::util::concurrent::atomic
