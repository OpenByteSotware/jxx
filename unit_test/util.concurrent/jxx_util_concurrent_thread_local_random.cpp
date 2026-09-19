#include <gtest/gtest.h>
#include <future>

#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.concurrent.ThreadLocalRandom.h"

namespace {
using jxx::util::concurrent::ThreadLocalRandom;

TEST(ThreadLocalRandomTest, CurrentIsStableWithinThread) {
    auto first = ThreadLocalRandom::current();
    auto second = ThreadLocalRandom::current();
    ASSERT_NE(first, nullptr);
    EXPECT_EQ(first.get(), second.get());
}

TEST(ThreadLocalRandomTest, CurrentIsDifferentAcrossThreads) {
    auto local = ThreadLocalRandom::current();
    auto future = std::async(std::launch::async, [] {
        return ThreadLocalRandom::current();
    });
    auto other = future.get();
    EXPECT_NE(local.get(), other.get());
}

TEST(ThreadLocalRandomTest, BoundedValuesRemainInRange) {
    auto random = ThreadLocalRandom::current();
    for (int i = 0; i < 256; ++i) {
        const auto value = random->nextInt(7, 19);
        EXPECT_GE(value, 7);
        EXPECT_LT(value, 19);
    }
}

TEST(ThreadLocalRandomTest, InvalidBoundsThrow) {
    auto random = ThreadLocalRandom::current();
    EXPECT_THROW(random->nextInt(4, 4), jxx::lang::IllegalArgumentException);
    EXPECT_THROW(random->nextLong(0), jxx::lang::IllegalArgumentException);
    EXPECT_THROW(random->nextDouble(0.0), jxx::lang::IllegalArgumentException);
}
}
