#include <gtest/gtest.h>

#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.PriorityQueue.h"

TEST(PriorityQueueParity, PollsElementsInNaturalOrder) {
    const auto queue = ::jxx::NEW<::jxx::util::PriorityQueue<::jxx::lang::Integer>>();
    EXPECT_TRUE(queue->offer(::jxx::NEW<::jxx::lang::Integer>(3)));
    EXPECT_TRUE(queue->offer(::jxx::NEW<::jxx::lang::Integer>(1)));
    EXPECT_TRUE(queue->offer(::jxx::NEW<::jxx::lang::Integer>(2)));

    EXPECT_EQ(1, queue->poll()->intValue());
    EXPECT_EQ(2, queue->poll()->intValue());
    EXPECT_EQ(3, queue->poll()->intValue());
    EXPECT_EQ(nullptr, queue->poll());
}

TEST(PriorityQueueParity, RejectsNullElements) {
    const auto queue = ::jxx::NEW<::jxx::util::PriorityQueue<::jxx::lang::Integer>>();
    EXPECT_THROW(queue->offer(nullptr), ::jxx::lang::NullPointerException);
}

TEST(PriorityQueueParity, IteratorIsFailFast) {
    const auto queue = ::jxx::NEW<::jxx::util::PriorityQueue<::jxx::lang::Integer>>();
    queue->offer(::jxx::NEW<::jxx::lang::Integer>(1));
    const auto iteratorValue = queue->iterator();
    queue->offer(::jxx::NEW<::jxx::lang::Integer>(2));
    EXPECT_THROW(iteratorValue->next(), ::jxx::util::ConcurrentModificationException);
}

TEST(PriorityQueueParity, RemoveRepairsHeap) {
    const auto queue = ::jxx::NEW<::jxx::util::PriorityQueue<::jxx::lang::Integer>>();
    const auto one = ::jxx::NEW<::jxx::lang::Integer>(1);
    const auto two = ::jxx::NEW<::jxx::lang::Integer>(2);
    const auto three = ::jxx::NEW<::jxx::lang::Integer>(3);
    queue->offer(three);
    queue->offer(one);
    queue->offer(two);

    EXPECT_TRUE(queue->remove(::jxx::CAST<::jxx::lang::Object>(two)));
    EXPECT_EQ(1, queue->poll()->intValue());
    EXPECT_EQ(3, queue->poll()->intValue());
}
