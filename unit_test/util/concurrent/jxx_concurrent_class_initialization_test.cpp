#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "util/concurrent/jxx.util.concurrent.CountDownLatch.h"
#include "util/concurrent/jxx.util.concurrent.CyclicBarrier.h"
#include "util/concurrent/jxx.util.concurrent.Executor.h"
#include "util/concurrent/jxx.util.concurrent.ExecutorService.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.ForkJoinPool.h"
#include "util/concurrent/jxx.util.concurrent.Phaser.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/concurrent/jxx.util.concurrent.Semaphore.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

TEST(JxxConcurrentClassInitialization, NonTemplateTypesHaveClassMetadata)
{
    ::jxx::initialize();
    EXPECT_NE(::jxx::util::concurrent::CountDownLatch::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::CyclicBarrier::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::Executor::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::ExecutorService::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::Executors::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::ForkJoinPool::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::Phaser::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::ScheduledThreadPoolExecutor::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::Semaphore::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::TimeUnit::Class(), nullptr);
}
