#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.Collections.h"

namespace {
using String = jxx::lang::String;
using ArrayList = jxx::util::ArrayList<String>;
static jxx::Ptr<String> S(const char* v) { return jxx::NEW<String>(v); }

TEST(CollectionsTest, ReverseSwapRotateAndFillUseJxxList) {
    auto concrete = jxx::NEW<ArrayList>();
    concrete->add(S("a")); concrete->add(S("b")); concrete->add(S("c"));
    auto list = jxx::CAST<jxx::util::List<String>>(concrete);
    jxx::util::Collections::reverse<String>(list);
    EXPECT_EQ(list->get(0)->utf8(), "c");
    jxx::util::Collections::swap<String>(list, 0, 2);
    EXPECT_EQ(list->get(0)->utf8(), "a");
    jxx::util::Collections::rotate<String>(list, 1);
    EXPECT_EQ(list->get(0)->utf8(), "c");
    jxx::util::Collections::fill<String>(list, S("x"));
    EXPECT_EQ(list->get(2)->utf8(), "x");
}

TEST(CollectionsTest, ReplaceAllReportsModification) {
    auto concrete = jxx::NEW<ArrayList>();
    concrete->add(S("a")); concrete->add(S("b")); concrete->add(S("a"));
    auto list = jxx::CAST<jxx::util::List<String>>(concrete);
    EXPECT_TRUE(jxx::util::Collections::replaceAll<String>(list, S("a"), S("x")));
    EXPECT_EQ(list->get(0)->utf8(), "x");
    EXPECT_EQ(list->get(2)->utf8(), "x");
}
} // namespace
