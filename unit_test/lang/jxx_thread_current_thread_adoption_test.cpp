#include <atomic>
#include <thread>

#include <gtest/gtest.h>

#include "lang/jxx.lang.Thread.h"

namespace {

TEST(ThreadCurrentThreadAdoptionTest, ReturnsStableWrapperForCallingNativeThread) {
    const auto first = ::jxx::lang::Thread::currentThread();
    const auto second = ::jxx::lang::Thread::currentThread();

    ASSERT_NE(nullptr, first);
    ASSERT_NE(nullptr, second);
    EXPECT_EQ(first.get(), second.get());
    EXPECT_TRUE(first->isAlive());
}

TEST(ThreadCurrentThreadAdoptionTest, InterruptedReportsAndClearsAdoptedThreadStatus) {
    const auto current = ::jxx::lang::Thread::currentThread();
    ASSERT_NE(nullptr, current);

    while (::jxx::lang::Thread::interrupted()) {
    }

    current->interrupt();

    EXPECT_TRUE(current->isInterrupted());
    EXPECT_TRUE(::jxx::lang::Thread::interrupted());
    EXPECT_FALSE(current->isInterrupted());
    EXPECT_FALSE(::jxx::lang::Thread::interrupted());
}

TEST(ThreadCurrentThreadAdoptionTest, NativeThreadsReceiveDistinctWrappers) {
    const auto callingThread = ::jxx::lang::Thread::currentThread();
    ASSERT_NE(nullptr, callingThread);

    std::atomic<::jxx::lang::Thread*> first{nullptr};
    std::atomic<::jxx::lang::Thread*> second{nullptr};
    std::atomic<bool> stable{false};

    std::thread nativeThread([&] {
        const auto adoptedFirst = ::jxx::lang::Thread::currentThread();
        const auto adoptedSecond = ::jxx::lang::Thread::currentThread();

        first.store(adoptedFirst.get(), std::memory_order_release);
        second.store(adoptedSecond.get(), std::memory_order_release);
        stable.store(
            adoptedFirst != nullptr &&
            adoptedFirst.get() == adoptedSecond.get(),
            std::memory_order_release);
    });

    nativeThread.join();

    EXPECT_TRUE(stable.load(std::memory_order_acquire));
    EXPECT_NE(nullptr, first.load(std::memory_order_acquire));
    EXPECT_EQ(
        first.load(std::memory_order_acquire),
        second.load(std::memory_order_acquire));
    EXPECT_NE(
        callingThread.get(),
        first.load(std::memory_order_acquire));
}

TEST(ThreadCurrentThreadAdoptionTest, InterruptStatusIsThreadLocal) {
    const auto callingThread = ::jxx::lang::Thread::currentThread();
    ASSERT_NE(nullptr, callingThread);

    while (::jxx::lang::Thread::interrupted()) {
    }

    std::atomic<bool> nativeFirst{false};
    std::atomic<bool> nativeSecond{true};

    std::thread nativeThread([&] {
        const auto adopted = ::jxx::lang::Thread::currentThread();
        if (adopted == nullptr) {
            return;
        }

        adopted->interrupt();
        nativeFirst.store(
            ::jxx::lang::Thread::interrupted(),
            std::memory_order_release);
        nativeSecond.store(
            ::jxx::lang::Thread::interrupted(),
            std::memory_order_release);
    });

    nativeThread.join();

    EXPECT_TRUE(nativeFirst.load(std::memory_order_acquire));
    EXPECT_FALSE(nativeSecond.load(std::memory_order_acquire));
    EXPECT_FALSE(callingThread->isInterrupted());
    EXPECT_FALSE(::jxx::lang::Thread::interrupted());
}

} // namespace
