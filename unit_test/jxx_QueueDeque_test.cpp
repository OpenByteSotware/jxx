#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Deque.h"
#include "util/jxx.util.LinkedList.h"
#include "util/jxx.util.Queue.h"

namespace {
using String = jxx::lang::String;
static jxx::Ptr<String> S(const char* v) { return jxx::NEW<String>(v); }

TEST(QueueTest, InterfaceDispatchUsesLinkedList) {
    auto concrete = jxx::NEW<jxx::util::LinkedList<String>>();
    auto queue = jxx::CAST<jxx::util::Queue<String>>(concrete);
    ASSERT_NE(queue, nullptr);
    EXPECT_TRUE(queue->offer(S("a")));
    EXPECT_TRUE(queue->offer(S("b")));
    EXPECT_EQ(queue->element()->utf8(), "a");
    EXPECT_EQ(queue->remove()->utf8(), "a");
    EXPECT_EQ(queue->poll()->utf8(), "b");
    EXPECT_EQ(queue->peek(), nullptr);
}

TEST(DequeTest, InterfaceDispatchSupportsBothEnds) {
    auto concrete = jxx::NEW<jxx::util::LinkedList<String>>();
    auto deque = jxx::CAST<jxx::util::Deque<String>>(concrete);
    ASSERT_NE(deque, nullptr);
    deque->addFirst(S("b"));
    deque->addFirst(S("a"));
    deque->addLast(S("c"));
    EXPECT_EQ(deque->peekFirst()->utf8(), "a");
    EXPECT_EQ(deque->peekLast()->utf8(), "c");
    EXPECT_EQ(deque->pollFirst()->utf8(), "a");
    EXPECT_EQ(deque->pollLast()->utf8(), "c");
}
} // namespace
