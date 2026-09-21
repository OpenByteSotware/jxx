#include <gtest/gtest.h>

#include <atomic>
#include <chrono>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.Runtime.h"
#include "lang/jxx.lang.Thread.h"

namespace {
using ::jxx::lang::IllegalArgumentException;
using ::jxx::lang::NullPointerException;
using ::jxx::lang::Runnable;
using ::jxx::lang::Runtime;
using ::jxx::lang::Thread;

class NoOpRunnable final
    : public ::jxx::lang::ClassBase<
          NoOpRunnable,
          ::jxx::lang::Object,
          Runnable> {
public:
    using JxxSuper = ::jxx::lang::Object;
    void run() override {}
};

class BlockingRunnable final
    : public ::jxx::lang::ClassBase<
          BlockingRunnable,
          ::jxx::lang::Object,
          Runnable> {
public:
    using JxxSuper = ::jxx::lang::Object;
    std::atomic<bool> entered{false};
    std::atomic<bool> release{false};
    void run() override {
        entered = true;
        while (!release.load()) Thread::yield();
    }
};

TEST(RuntimeShutdownHookTest, RuntimeIsSingleton) {
    EXPECT_EQ(Runtime::getRuntime().get(), Runtime::getRuntime().get());
}

TEST(RuntimeShutdownHookTest, RejectsNullAndDuplicateHooks) {
    const auto runtime = Runtime::getRuntime();
    const ::jxx::Ptr<Thread> nullHook;
    EXPECT_THROW(runtime->addShutdownHook(nullHook), NullPointerException);
    EXPECT_THROW(runtime->removeShutdownHook(nullHook), NullPointerException);

    const auto hook = ::jxx::NEW<Thread>(
        ::jxx::CAST<Runnable>(::jxx::NEW<NoOpRunnable>()));
    runtime->addShutdownHook(hook);
    EXPECT_THROW(runtime->addShutdownHook(hook), IllegalArgumentException);
    EXPECT_TRUE(runtime->removeShutdownHook(hook));
}

TEST(RuntimeShutdownHookTest, RemoveReportsRegistrationState) {
    const auto runtime = Runtime::getRuntime();
    const auto hook = ::jxx::NEW<Thread>(
        ::jxx::CAST<Runnable>(::jxx::NEW<NoOpRunnable>()));

    EXPECT_FALSE(runtime->removeShutdownHook(hook));
    runtime->addShutdownHook(hook);
    EXPECT_TRUE(runtime->removeShutdownHook(hook));
    EXPECT_FALSE(runtime->removeShutdownHook(hook));
}

TEST(RuntimeShutdownHookTest, RejectsRunningHook) {
    const auto runtime = Runtime::getRuntime();
    const auto task = ::jxx::NEW<BlockingRunnable>();
    const auto hook = ::jxx::NEW<Thread>(::jxx::CAST<Runnable>(task));
    hook->start();
    while (!task->entered.load()) Thread::yield();

    EXPECT_THROW(runtime->addShutdownHook(hook), IllegalArgumentException);
    task->release = true;
    hook->join();
}

TEST(RuntimeShutdownHookTest, RejectsTerminatedHook) {
    const auto runtime = Runtime::getRuntime();
    const auto hook = ::jxx::NEW<Thread>(
        ::jxx::CAST<Runnable>(::jxx::NEW<NoOpRunnable>()));
    hook->start();
    hook->join();

    EXPECT_EQ(Thread::State::TERMINATED, hook->getState());
    EXPECT_THROW(runtime->addShutdownHook(hook), IllegalArgumentException);
}

TEST(RuntimeShutdownHookTest, UnstartedHookRemainsNewWhileRegistered) {
    const auto runtime = Runtime::getRuntime();
    const auto hook = ::jxx::NEW<Thread>(
        ::jxx::CAST<Runnable>(::jxx::NEW<NoOpRunnable>()));
    runtime->addShutdownHook(hook);
    EXPECT_EQ(Thread::State::NEW, hook->getState());
    EXPECT_FALSE(hook->isAlive());
    EXPECT_TRUE(runtime->removeShutdownHook(hook));
}

} // namespace
