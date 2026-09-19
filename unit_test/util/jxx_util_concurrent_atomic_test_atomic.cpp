#include <gtest/gtest.h>

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.String.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicIntegerArray.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicLongArray.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicMarkableReference.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicReferenceArray.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicStampedReference.h"

namespace {
using jxx::lang::String;
using jxx::util::concurrent::atomic::AtomicIntegerArray;
using jxx::util::concurrent::atomic::AtomicLongArray;
using jxx::util::concurrent::atomic::AtomicMarkableReference;
using jxx::util::concurrent::atomic::AtomicReferenceArray;
using jxx::util::concurrent::atomic::AtomicStampedReference;

TEST(AtomicArrayTest, IntegerReadModifyWriteOperations) {
    AtomicIntegerArray values(2);
    EXPECT_EQ(values.length(), 2);
    EXPECT_EQ(values.getAndAdd(0, 4), 0);
    EXPECT_EQ(values.incrementAndGet(0), 5);
    EXPECT_TRUE(values.compareAndSet(0, 5, 9));
    EXPECT_EQ(values.get(0), 9);
}

TEST(AtomicArrayTest, LongBoundsCheck) {
    AtomicLongArray values(1);
    EXPECT_THROW(values.get(1), jxx::lang::IndexOutOfBoundsException);
}

TEST(AtomicReferenceArrayTest, CompareAndSetUsesReferenceIdentity) {
    AtomicReferenceArray<String> values(1);
    auto first = jxx::NEW<String>("same");
    auto equivalent = jxx::NEW<String>("same");
    auto replacement = jxx::NEW<String>("new");
    values.set(0, first);
    EXPECT_FALSE(values.compareAndSet(0, equivalent, replacement));
    EXPECT_TRUE(values.compareAndSet(0, first, replacement));
    EXPECT_EQ(values.get(0).get(), replacement.get());
}

TEST(AtomicPairTest, MarkAndStampAreUpdatedWithReference) {
    auto first = jxx::NEW<String>("first");
    auto second = jxx::NEW<String>("second");
    AtomicMarkableReference<String> marked(first, false);
    EXPECT_TRUE(marked.compareAndSet(first, second, false, true));
    EXPECT_TRUE(marked.isMarked());
    AtomicStampedReference<String> stamped(first, 3);
    EXPECT_TRUE(stamped.compareAndSet(first, second, 3, 4));
    EXPECT_EQ(stamped.getStamp(), 4);
}
}
