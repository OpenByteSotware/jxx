#include <gtest/gtest.h>
#include <chrono>
#include <future>
#include <thread>

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.concurrent.ArrayBlockingQueue.h"
#include "util/jxx.util.concurrent.LinkedBlockingQueue.h"
#include "util/jxx.util.concurrent.SynchronousQueue.h"

namespace {
using namespace std::chrono_literals;
using S = jxx::lang::String;

TEST(ArrayBlockingQueueTest, CapacityAndFifoBehavior) {
    auto queue = jxx::NEW<jxx::util::concurrent::ArrayBlockingQueue<S>>(2);
    EXPECT_TRUE(queue->offer(jxx::NEW<S>("a")));
    EXPECT_TRUE(queue->offer(jxx::NEW<S>("b")));
    EXPECT_FALSE(queue->offer(jxx::NEW<S>("c")));
    EXPECT_EQ(queue->remainingCapacity(), 0);
    EXPECT_EQ(queue->poll()->utf8(), "a");
    EXPECT_EQ(queue->poll()->utf8(), "b");
}

TEST(ArrayBlockingQueueTest, RejectsNullElements) {
    auto queue = jxx::NEW<jxx::util::concurrent::ArrayBlockingQueue<S>>(1);
    EXPECT_THROW(queue->offer(nullptr), jxx::lang::NullPointerException);
}

TEST(LinkedBlockingQueueTest, TakeWaitsForProducer) {
    auto queue = jxx::NEW<jxx::util::concurrent::LinkedBlockingQueue<S>>(1);
    auto consumer = std::async(std::launch::async, [&] { return queue->take(); });
    std::this_thread::sleep_for(10ms);
    queue->put(jxx::NEW<S>("handoff"));
    EXPECT_EQ(consumer.get()->utf8(), "handoff");
}

TEST(SynchronousQueueTest, HasNoInternalCapacity) {
    auto queue = jxx::NEW<jxx::util::concurrent::SynchronousQueue<S>>();
    EXPECT_EQ(queue->size(), 0);
    EXPECT_EQ(queue->remainingCapacity(), 0);
    EXPECT_EQ(queue->peek(), nullptr);
    EXPECT_FALSE(queue->contains(jxx::CAST<jxx::lang::Object>(jxx::NEW<S>("x"))));
}

TEST(SynchronousQueueTest, ProducerAndConsumerRendezvous) {
    auto queue = jxx::NEW<jxx::util::concurrent::SynchronousQueue<S>>();
    auto consumer = std::async(std::launch::async, [&] { return queue->take(); });
    auto producer = std::async(std::launch::async, [&] {
        queue->put(jxx::NEW<S>("value"));
    });
    EXPECT_EQ(consumer.get()->utf8(), "value");
    producer.get();
}
}
