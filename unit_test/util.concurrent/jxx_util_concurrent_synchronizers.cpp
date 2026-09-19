#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <future>
#include <thread>

#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.concurrent.CountDownLatch.h"
#include "util/jxx.util.concurrent.CyclicBarrier.h"
#include "util/jxx.util.concurrent.Phaser.h"
#include "util/jxx.util.concurrent.Semaphore.h"

namespace {
using namespace std::chrono_literals;
using jxx::util::concurrent::CountDownLatch;
using jxx::util::concurrent::CyclicBarrier;
using jxx::util::concurrent::Phaser;
using jxx::util::concurrent::Semaphore;

TEST(SemaphoreTest, AcquireAndReleaseUpdatePermitCount) {
    Semaphore semaphore(2);
    EXPECT_EQ(semaphore.availablePermits(), 2);
    EXPECT_TRUE(semaphore.tryAcquire());
    EXPECT_EQ(semaphore.availablePermits(), 1);
    semaphore.release();
    EXPECT_EQ(semaphore.availablePermits(), 2);
}

TEST(SemaphoreTest, RejectsNegativeInitialPermitCount) {
    EXPECT_THROW(Semaphore(-1), jxx::lang::IllegalArgumentException);
}

TEST(CountDownLatchTest, AwaitUnblocksAtZero) {
    auto latch = jxx::NEW<CountDownLatch>(1);
    std::atomic<bool> released{false};
    auto waiter = std::async(std::launch::async, [&] {
        latch->await();
        released.store(true);
    });
    std::this_thread::sleep_for(10ms);
    EXPECT_FALSE(released.load());
    latch->countDown();
    waiter.get();
    EXPECT_TRUE(released.load());
    EXPECT_EQ(latch->getCount(), 0);
}

TEST(CyclicBarrierTest, ReusesAfterTrip) {
    auto barrier = jxx::NEW<CyclicBarrier>(2);
    auto first = std::async(std::launch::async, [&] { return barrier->await(); });
    auto secondIndex = barrier->await();
    auto firstIndex = first.get();
    EXPECT_NE(firstIndex, secondIndex);
    EXPECT_FALSE(barrier->isBroken());
    EXPECT_EQ(barrier->getNumberWaiting(), 0);
}

TEST(PhaserTest, RegisteredPartiesAdvanceTogether) {
    auto phaser = jxx::NEW<Phaser>(2);
    auto first = std::async(std::launch::async, [&] {
        return phaser->arriveAndAwaitAdvance();
    });
    const auto secondPhase = phaser->arriveAndAwaitAdvance();
    const auto firstPhase = first.get();
    EXPECT_EQ(firstPhase, secondPhase);
    EXPECT_EQ(phaser->getPhase(), 1);
}
}
