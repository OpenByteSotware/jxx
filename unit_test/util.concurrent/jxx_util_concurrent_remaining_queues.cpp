#include <gtest/gtest.h>
#include <future>

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.concurrent.Exchanger.h"
#include "util/jxx.util.concurrent.LinkedTransferQueue.h"
#include "util/jxx.util.concurrent.PriorityBlockingQueue.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace {
using S = jxx::lang::String;
using jxx::util::concurrent::Exchanger;
using jxx::util::concurrent::LinkedTransferQueue;
using jxx::util::concurrent::PriorityBlockingQueue;
using jxx::util::concurrent::TimeUnit;

TEST(ExchangerTest, TwoThreadsSwapValues) {
    auto exchanger = jxx::NEW<Exchanger<S>>();
    auto first = std::async(std::launch::async, [&] {
        return exchanger->exchange(jxx::NEW<S>("first"));
    });
    auto second = exchanger->exchange(jxx::NEW<S>("second"));
    EXPECT_EQ(second->utf8(), "first");
    EXPECT_EQ(first.get()->utf8(), "second");
}

TEST(ExchangerTest, TimedExchangeWithoutPartnerTimesOut) {
    auto exchanger = jxx::NEW<Exchanger<S>>();
    EXPECT_THROW(
        exchanger->exchange(jxx::NEW<S>("value"), 5, TimeUnit::MILLISECONDS()),
        jxx::util::concurrent::TimeoutException);
}

TEST(LinkedTransferQueueTest, TransferWaitsForConsumer) {
    auto queue = jxx::NEW<LinkedTransferQueue<S>>();
    auto producer = std::async(std::launch::async, [&] {
        queue->transfer(jxx::NEW<S>("value"));
        return true;
    });
    EXPECT_EQ(queue->take()->utf8(), "value");
    EXPECT_TRUE(producer.get());
}

TEST(LinkedTransferQueueTest, TimedTransferRemovesUnconsumedElement) {
    auto queue = jxx::NEW<LinkedTransferQueue<S>>();
    EXPECT_FALSE(queue->tryTransfer(
        jxx::NEW<S>("orphan"), 5, TimeUnit::MILLISECONDS()));
    EXPECT_EQ(queue->poll(), nullptr);
}

TEST(PriorityBlockingQueueTest, RetrievesNaturalMinimumFirst) {
    auto queue = jxx::NEW<PriorityBlockingQueue<S>>();
    queue->offer(jxx::NEW<S>("c"));
    queue->offer(jxx::NEW<S>("a"));
    queue->offer(jxx::NEW<S>("b"));
    EXPECT_EQ(queue->poll()->utf8(), "a");
    EXPECT_EQ(queue->poll()->utf8(), "b");
    EXPECT_EQ(queue->poll()->utf8(), "c");
}
}
