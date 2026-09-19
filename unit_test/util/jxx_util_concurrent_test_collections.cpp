#include <gtest/gtest.h>
#include <atomic>
#include <thread>
#include <vector>

#include "lang/jxx.lang.String.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentHashMap.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentLinkedDeque.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentLinkedQueue.h"
#include "util/concurrent/jxx.util.concurrent.CopyOnWriteArrayList.h"
#include "util/concurrent/jxx.util.concurrent.CopyOnWriteArraySet.h"

namespace {
using S = jxx::lang::String;
using Map = jxx::util::concurrent::ConcurrentHashMap<S, S>;

TEST(ConcurrentLinkedQueueTest, PreservesFifoOrdering) {
    auto queue = jxx::NEW<jxx::util::concurrent::ConcurrentLinkedQueue<S>>();
    queue->offer(jxx::NEW<S>("a"));
    queue->offer(jxx::NEW<S>("b"));
    EXPECT_EQ(queue->poll()->utf8(), "a");
    EXPECT_EQ(queue->poll()->utf8(), "b");
    EXPECT_EQ(queue->poll(), nullptr);
}

TEST(ConcurrentLinkedDequeTest, SupportsBothEnds) {
    auto deque = jxx::NEW<jxx::util::concurrent::ConcurrentLinkedDeque<S>>();
    deque->addFirst(jxx::NEW<S>("middle"));
    deque->addFirst(jxx::NEW<S>("first"));
    deque->addLast(jxx::NEW<S>("last"));
    EXPECT_EQ(deque->pollFirst()->utf8(), "first");
    EXPECT_EQ(deque->pollLast()->utf8(), "last");
    EXPECT_EQ(deque->poll()->utf8(), "middle");
}

TEST(CopyOnWriteArrayListTest, IteratorRetainsCreationSnapshot) {
    auto list = jxx::NEW<jxx::util::concurrent::CopyOnWriteArrayList<S>>();
    list->add(jxx::NEW<S>("before"));
    auto iterator = list->iterator();
    list->add(jxx::NEW<S>("after"));
    ASSERT_TRUE(iterator->hasNext());
    EXPECT_EQ(iterator->next()->utf8(), "before");
    EXPECT_FALSE(iterator->hasNext());
}

TEST(CopyOnWriteArraySetTest, SuppressesDuplicates) {
    auto set = jxx::NEW<jxx::util::concurrent::CopyOnWriteArraySet<S>>();
    auto value = jxx::NEW<S>("value");
    EXPECT_TRUE(set->add(value));
    EXPECT_FALSE(set->add(value));
    EXPECT_EQ(set->size(), 1);
}

TEST(ConcurrentHashMapTest, AtomicConditionalOperations) {
    auto map = jxx::NEW<Map>();
    auto key = jxx::NEW<S>("key");
    auto first = jxx::NEW<S>("first");
    auto second = jxx::NEW<S>("second");
    EXPECT_EQ(map->putIfAbsent(key, first), nullptr);
    EXPECT_EQ(map->putIfAbsent(key, second).get(), first.get());
    EXPECT_TRUE(map->replace(key, first, second));
    EXPECT_EQ(map->get(jxx::CAST<jxx::lang::Object>(key)).get(), second.get());
}
}
