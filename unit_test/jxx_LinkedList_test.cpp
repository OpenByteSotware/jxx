#include <gtest/gtest.h>
#include <type_traits>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Deque.h"
#include "util/jxx.util.LinkedList.h"
#include "util/jxx.util.Queue.h"

namespace {
using String = jxx::lang::String;
using Linked = jxx::util::LinkedList<String>;
static jxx::Ptr<String> S(const char* v) { return jxx::NEW<String>(v); }

TEST(LinkedListTest, ImplementsListQueueAndDeque) {
    static_assert(std::is_base_of_v<jxx::util::List<String>, Linked>);
    static_assert(std::is_base_of_v<jxx::util::Queue<String>, Linked>);
    static_assert(std::is_base_of_v<jxx::util::Deque<String>, Linked>);
    SUCCEED();
}

TEST(LinkedListTest, DequeAndQueueOperationsMatchJava) {
    auto list = jxx::NEW<Linked>();
    list->addFirst(S("b"));
    list->addFirst(S("a"));
    list->addLast(S("c"));
    EXPECT_EQ(list->getFirst()->utf8(), "a");
    EXPECT_EQ(list->getLast()->utf8(), "c");
    EXPECT_EQ(list->removeFirst()->utf8(), "a");
    EXPECT_EQ(list->removeLast()->utf8(), "c");
    EXPECT_EQ(list->poll()->utf8(), "b");
    EXPECT_EQ(list->poll(), nullptr);
    EXPECT_THROW(list->remove(), jxx::util::NoSuchElementException);
}

TEST(LinkedListTest, StackAndOccurrenceOperationsWork) {
    auto list = jxx::NEW<Linked>();
    list->add(S("x")); list->add(S("y")); list->add(S("x"));
    EXPECT_TRUE(list->removeLastOccurrence(jxx::CAST<jxx::lang::Object>(S("x"))));
    EXPECT_EQ(list->size(), 2);
    list->push(S("top"));
    EXPECT_EQ(list->pop()->utf8(), "top");
}

TEST(LinkedListTest, ListIteratorAndDescendingIteratorWork) {
    auto list = jxx::NEW<Linked>();
    list->add(S("a")); list->add(S("b")); list->add(S("c"));
    auto it = list->listIterator(1);
    EXPECT_EQ(it->previous()->utf8(), "a");
    EXPECT_EQ(it->next()->utf8(), "a");
    EXPECT_EQ(it->next()->utf8(), "b");
    auto descending = list->descendingIterator();
    EXPECT_EQ(descending->next()->utf8(), "c");
    EXPECT_EQ(descending->next()->utf8(), "b");
}

TEST(LinkedListTest, SubListMutatesBackingList) {
    auto list = jxx::NEW<Linked>();
    list->add(S("a")); list->add(S("b")); list->add(S("c"));
    auto view = list->subList(1, 3);
    view->set(0, S("B"));
    EXPECT_EQ(list->get(1)->utf8(), "B");
    view->clear();
    EXPECT_EQ(list->size(), 1);
    EXPECT_EQ(list->get(0)->utf8(), "a");
}
} // namespace
