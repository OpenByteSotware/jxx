#include "util/concurrent/atomic/jxx.util.concurrent.atomic.ClassInitialization.h"

#include <mutex>

#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"

#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicBoolean.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicInteger.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicIntegerArray.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicLong.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicLongArray.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicReference.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicReferenceArray.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicMarkableReference.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicStampedReference.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.DoubleAccumulator.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.DoubleAdder.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.LongAccumulator.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.LongAdder.h"

namespace jxx::util::concurrent::atomic {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
            ::jxx::lang::class_info_detail::registerClassInfo<AtomicBoolean>();
            ::jxx::lang::class_info_detail::registerClassInfo<AtomicInteger>();
            ::jxx::lang::class_info_detail::registerClassInfo<AtomicIntegerArray>();
            ::jxx::lang::class_info_detail::registerClassInfo<AtomicLong>();
            ::jxx::lang::class_info_detail::registerClassInfo<AtomicLongArray>();
            ::jxx::lang::class_info_detail::registerClassInfo<DoubleAccumulator>();
            ::jxx::lang::class_info_detail::registerClassInfo<DoubleAdder>();
            ::jxx::lang::class_info_detail::registerClassInfo<LongAccumulator>();
            ::jxx::lang::class_info_detail::registerClassInfo<LongAdder>();
        (void)::jxx::lang::ClassBase<
            AtomicBoolean, ::jxx::lang::Object, ::jxx::io::SerializableI>::Class();
        ::jxx::io::serialization::registerSerializableClass<AtomicBoolean>(
            static_cast<::jxx::lang::jlong>(4654671469794556979LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            AtomicBoolean, 1U, 2U>();
        (void)::jxx::lang::ClassBase<
            AtomicInteger, ::jxx::lang::Number, ::jxx::io::SerializableI>::Class();
        ::jxx::io::serialization::registerSerializableClass<AtomicInteger>(
            static_cast<::jxx::lang::jlong>(6214790243416807050LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            AtomicInteger, 1U, 2U>();
        (void)::jxx::lang::ClassBase<AtomicIntegerArray, ::jxx::lang::Object, ::jxx::io::SerializableI>::Class();
        ::jxx::io::serialization::registerSerializableClass<AtomicIntegerArray>(
            static_cast<::jxx::lang::jlong>(2862133569453604235LL),
            [] { return ::jxx::NEW<AtomicIntegerArray>(0); });
        (void)::jxx::lang::ClassBase<
            AtomicLong, ::jxx::lang::Number, ::jxx::io::SerializableI>::Class();
        ::jxx::io::serialization::registerSerializableClass<AtomicLong>(
            static_cast<::jxx::lang::jlong>(1927816293512124184LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            AtomicLong, 1U, 2U>();
        (void)::jxx::lang::ClassBase<AtomicLongArray, ::jxx::lang::Object, ::jxx::io::SerializableI>::Class();
        ::jxx::io::serialization::registerSerializableClass<AtomicLongArray>(
            static_cast<::jxx::lang::jlong>(-2308431214976778248LL),
            [] { return ::jxx::NEW<AtomicLongArray>(0); });
        using ErasedAtomicReference = AtomicReference<::jxx::lang::Object>;
        ::jxx::lang::class_info_detail::registerClassInfo<ErasedAtomicReference>();
        ::jxx::io::serialization::registerSerializableClass<ErasedAtomicReference>(
            static_cast<::jxx::lang::jlong>(-1848883965231344442LL));
        (void)AtomicMarkableReference<::jxx::lang::Object>::Class();
        (void)AtomicStampedReference<::jxx::lang::Object>::Class();

        using ErasedAtomicReferenceArray = AtomicReferenceArray<::jxx::lang::Object>;
        ::jxx::lang::class_info_detail::registerClassInfo<ErasedAtomicReferenceArray>();
        ::jxx::io::serialization::registerSerializableClass<ErasedAtomicReferenceArray>(
            static_cast<::jxx::lang::jlong>(-6209656149925076980LL),
            [] { return ::jxx::NEW<ErasedAtomicReferenceArray>(0); });
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            ErasedAtomicReferenceArray, 1U, 2U>();
        (void)::jxx::lang::ClassBase<DoubleAccumulator, ::jxx::lang::Number, ::jxx::io::SerializableI>::Class();
        ::jxx::io::serialization::registerSerializableClass<DoubleAccumulator>(static_cast<::jxx::lang::jlong>(7249069246863182397LL), [] {
            return DoubleAccumulator::createForDeserialization();
        });
        (void)::jxx::lang::ClassBase<DoubleAdder, ::jxx::lang::Number, ::jxx::io::SerializableI>::Class();
        ::jxx::io::serialization::registerSerializableClass<DoubleAdder>(static_cast<::jxx::lang::jlong>(7249069246863182397LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            DoubleAdder, 1U, 2U>();
        (void)::jxx::lang::ClassBase<LongAccumulator, ::jxx::lang::Number, ::jxx::io::SerializableI>::Class();
        ::jxx::io::serialization::registerSerializableClass<LongAccumulator>(static_cast<::jxx::lang::jlong>(7249069246863182397LL), [] {
            return LongAccumulator::createForDeserialization();
        });
        (void)::jxx::lang::ClassBase<LongAdder, ::jxx::lang::Number, ::jxx::io::SerializableI>::Class();
        ::jxx::io::serialization::registerSerializableClass<LongAdder>(static_cast<::jxx::lang::jlong>(7249069246863182397LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            LongAdder, 1U, 2U>();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::util::concurrent::atomic
