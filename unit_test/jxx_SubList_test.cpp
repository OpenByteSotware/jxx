#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ArrayList.h"

namespace {
using String = jxx::lang::String;
static jxx::Ptr<String> S(const char* v) { return jxx::NEW<String>(v); }

TEST(SubListTest, NestedViewUsesRootOffsets) {
    auto root = jxx::NEW<jxx::util::ArrayList<String>>();
    root->add(S("a")); root->add(S("b")); root->add(S("c")); root->add(S("d"));
    auto first = root->subList(1, 4);
    auto second = first->subList(1, 3);
    EXPECT_EQ(second->get(0)->utf8(), "c");
    second->set(1, S("D"));
    EXPECT_EQ(root->get(3)->utf8(), "D");
}

TEST(SubListTest, StructuralRootChangeIsFailFast) {
    auto root = jxx::NEW<jxx::util::ArrayList<String>>();
    root->add(S("a")); root->add(S("b"));
    auto view = root->subList(0, 1);
    root->add(S("c"));
    EXPECT_THROW(view->size(), jxx::util::ConcurrentModificationException);
}
} // namespace
