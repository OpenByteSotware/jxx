#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicBoolean.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicInteger.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicIntegerArray.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicLong.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicLongArray.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.ClassInitialization.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.DoubleAccumulator.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.DoubleAdder.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.LongAccumulator.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.LongAdder.h"

TEST(JxxAtomicClassInitialization, NonTemplateMetadataTypesAreInitialized)
{
    namespace A = ::jxx::util::concurrent::atomic;
    ::jxx::initialize();

    EXPECT_NE((::jxx::lang::ClassBase<A::AtomicBoolean, ::jxx::lang::Object>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<A::AtomicInteger, ::jxx::lang::Number>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<A::AtomicIntegerArray, ::jxx::lang::Object, ::jxx::io::SerializableI>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<A::AtomicLong, ::jxx::lang::Number>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<A::AtomicLongArray, ::jxx::lang::Object, ::jxx::io::SerializableI>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<A::DoubleAccumulator, ::jxx::lang::Number, ::jxx::io::SerializableI>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<A::DoubleAdder, ::jxx::lang::Number, ::jxx::io::SerializableI>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<A::LongAccumulator, ::jxx::lang::Number, ::jxx::io::SerializableI>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<A::LongAdder, ::jxx::lang::Number, ::jxx::io::SerializableI>::Class()), nullptr);
}

TEST(JxxAtomicClassInitialization, RepeatedInitializationIsIdempotent)
{
    namespace A = ::jxx::util::concurrent::atomic;
    const auto before = ::jxx::lang::ClassBase<A::AtomicLong, ::jxx::lang::Number>::Class();
    EXPECT_NO_THROW(A::initializeClasses());
    EXPECT_NO_THROW(A::initializeClasses());
    EXPECT_EQ(before, (::jxx::lang::ClassBase<A::AtomicLong, ::jxx::lang::Number>::Class()));
}
