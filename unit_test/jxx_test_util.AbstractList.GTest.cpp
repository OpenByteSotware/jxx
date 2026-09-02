#include <gtest/gtest.h>
#include <type_traits>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.AbstractList.h"
#include "util/jxx.util.ArrayList.h"

namespace {
using String = jxx::lang::String;
using Concrete = jxx::util::ArrayList<String>;
static jxx::Ptr<String> S(const char* v) { return jxx::NEW<String>(v); }

TEST(AbstractListTest, ConcreteListUsesAbstractListDefaults) {
    static_assert(std::is_base_of_v<jxx::util::AbstractList<String>, Concrete>);
    auto list = jxx::NEW<Concrete>();
    EXPECT_TRUE(list->add(S("a")));
    EXPECT_TRUE(list->add(S("b")));
    auto iterator = list->iterator();
    EXPECT_EQ(iterator->next()->utf8(), "a");
    iterator->remove();
    EXPECT_EQ(list->size(), 1);
    EXPECT_EQ(list->get(0)->utf8(), "b");
}

TEST(AbstractListTest, IndexedListIteratorOverloadIsVisible) {
    auto list = jxx::NEW<Concrete>();
    list->add(S("a")); list->add(S("b"));
    auto iterator = list->listIterator(1);
    EXPECT_EQ(iterator->previousIndex(), 0);
    EXPECT_EQ(iterator->nextIndex(), 1);
    EXPECT_EQ(iterator->previous()->utf8(), "a");
}
} // namespace
