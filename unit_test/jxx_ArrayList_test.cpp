#include <gtest/gtest.h>
#include <type_traits>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.RandomAccess.h"

namespace {
using String = jxx::lang::String;
using ListType = jxx::util::ArrayList<String>;
static jxx::Ptr<String> S(const char* v) { return jxx::NEW<String>(v); }

TEST(ArrayListTest, JavaHierarchyAndCoreOperations) {
    static_assert(std::is_base_of_v<jxx::util::List<String>, ListType>);
    static_assert(std::is_base_of_v<jxx::util::RandomAccess, ListType>);
    auto list = jxx::NEW<ListType>();
    EXPECT_TRUE(list->isEmpty());
    EXPECT_TRUE(list->add(S("a")));
    list->add(1, S("c"));
    list->add(1, S("b"));
    ASSERT_EQ(list->size(), 3);
    EXPECT_EQ(list->get(0)->utf8(), "a");
    EXPECT_EQ(list->get(1)->utf8(), "b");
    EXPECT_EQ(list->get(2)->utf8(), "c");
    auto old = list->set(1, S("B"));
    EXPECT_EQ(old->utf8(), "b");
    EXPECT_EQ(list->remove(1)->utf8(), "B");
    EXPECT_EQ(list->size(), 2);
}

TEST(ArrayListTest, SupportsNullAndLogicalSearch) {
    auto list = jxx::NEW<ListType>();
    list->add(nullptr);
    list->add(S("same"));
    EXPECT_EQ(list->indexOf(nullptr), 0);
    EXPECT_EQ(list->indexOf(jxx::CAST<jxx::lang::Object>(S("same"))), 1);
    EXPECT_TRUE(list->contains(nullptr));
}

TEST(ArrayListTest, IteratorIsFailFastAndMutable) {
    auto list = jxx::NEW<ListType>();
    list->add(S("a"));
    list->add(S("b"));
    auto it = list->listIterator();
    EXPECT_EQ(it->next()->utf8(), "a");
    it->set(S("A"));
    it->add(S("x"));
    EXPECT_EQ(list->get(0)->utf8(), "A");
    EXPECT_EQ(list->get(1)->utf8(), "x");
    list->add(S("c"));
    EXPECT_THROW(it->next(), jxx::util::ConcurrentModificationException);
}

TEST(ArrayListTest, SubListIsBackedByRoot) {
    auto list = jxx::NEW<ListType>();
    list->add(S("a")); list->add(S("b")); list->add(S("c"));
    auto view = list->subList(1, 3);
    ASSERT_NE(view, nullptr);
    EXPECT_EQ(view->size(), 2);
    EXPECT_EQ(view->set(0, S("B"))->utf8(), "b");
    EXPECT_EQ(list->get(1)->utf8(), "B");
    view->remove(1);
    EXPECT_EQ(list->size(), 2);
}

TEST(ArrayListTest, CloneHasIndependentStructureSharedElements) {
    auto list = jxx::NEW<ListType>();
    auto value = S("shared");
    list->add(value);
    auto clone = jxx::CAST<ListType>(list->clone());
    ASSERT_NE(clone, nullptr);
    EXPECT_NE(clone.get(), list.get());
    EXPECT_EQ(clone->get(0).get(), value.get());
    clone->add(S("new"));
    EXPECT_EQ(list->size(), 1);
}
} // namespace
