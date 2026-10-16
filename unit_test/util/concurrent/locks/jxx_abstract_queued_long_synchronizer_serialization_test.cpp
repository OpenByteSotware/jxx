#include <limits>

#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.Thread.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.AbstractQueuedLongSynchronizer.h"

namespace {

class TestLongSynchronizer final
    : public ::jxx::util::concurrent::locks::AbstractQueuedLongSynchronizer {
public:
    TestLongSynchronizer() = default;

    void state(::jxx::lang::jlong value) { setState(value); }
    ::jxx::lang::jlong state() const { return getState(); }

    void owner(const ::jxx::Ptr<::jxx::lang::Thread>& value) {
        setExclusiveOwnerThread(value);
    }

    ::jxx::Ptr<::jxx::lang::Thread> owner() const {
        return getExclusiveOwnerThread();
    }
};

TEST(AbstractQueuedLongSynchronizerSerialization,
    RoundTripsLongStateAndClearsTransientOwnerAndQueue) {
    constexpr ::jxx::lang::jlong expected = 0x123456789ABCDELL;
    auto original = ::jxx::NEW<TestLongSynchronizer>();
    original->state(expected);

    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    original->writeObject(output);
    output->flush();

    auto restored = ::jxx::NEW<TestLongSynchronizer>();
    restored->state(-1);
    restored->owner(::jxx::lang::Thread::currentThread());
    auto byteInput = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
        bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(byteInput);
    restored->readObject(input);

    EXPECT_EQ(expected, restored->state());
    EXPECT_EQ(nullptr, restored->owner());
    EXPECT_FALSE(restored->hasQueuedThreads());
    EXPECT_EQ(0, restored->getQueueLength());
}

TEST(AbstractQueuedLongSynchronizerSerialization,
    PreservesNegativeAndBoundaryLongStates) {
    for (const auto expected : {
             static_cast<::jxx::lang::jlong>(-1),
             (::std::numeric_limits<::jxx::lang::jlong>::min)(),
             (::std::numeric_limits<::jxx::lang::jlong>::max)()}) {
        auto original = ::jxx::NEW<TestLongSynchronizer>();
        original->state(expected);
        auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
        auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
        original->writeObject(output);
        output->flush();
        auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
            ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
        auto restored = ::jxx::NEW<TestLongSynchronizer>();
        restored->readObject(input);
        EXPECT_EQ(expected, restored->state());
    }
}

TEST(AbstractQueuedLongSynchronizerSerialization,
    NoDataRestoresZeroAndClearsOwner) {
    auto value = ::jxx::NEW<TestLongSynchronizer>();
    value->state(99);
    value->owner(::jxx::lang::Thread::currentThread());
    value->readObjectNoData();
    EXPECT_EQ(0, value->state());
    EXPECT_EQ(nullptr, value->owner());
    EXPECT_FALSE(value->hasQueuedThreads());
}

TEST(AbstractQueuedLongSynchronizerSerialization, RejectsNullStreams) {
    auto value = ::jxx::NEW<TestLongSynchronizer>();
    EXPECT_THROW(value->writeObject(nullptr), ::jxx::lang::NullPointerException);
    EXPECT_THROW(value->readObject(nullptr), ::jxx::lang::NullPointerException);
}

} // namespace
