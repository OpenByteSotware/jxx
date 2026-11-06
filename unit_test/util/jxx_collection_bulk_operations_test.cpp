#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.wildcard.CollectionAdapters.h"

namespace {
using JxxString = ::jxx::lang::String;
using StringList = ::jxx::util::ArrayList<JxxString>;

::jxx::Ptr<StringList> listOf(const char* first, const char* second = nullptr,
                              const char* third = nullptr) {
    auto result = ::jxx::NEW<StringList>();
    result->add(::jxx::NEW<JxxString>(first));
    if (second != nullptr) result->add(::jxx::NEW<JxxString>(second));
    if (third != nullptr) result->add(::jxx::NEW<JxxString>(third));
    return result;
}

::jxx::Ptr<::jxx::util::wildcard::CollectionAny> anyView(
    const ::jxx::Ptr<StringList>& list) {
    auto collection = ::jxx::CAST<::jxx::util::Collection<JxxString>>(list);
    auto view = ::jxx::NEW<
        ::jxx::util::wildcard::CollectionAnyView<JxxString>>(collection);
    return ::jxx::CAST<::jxx::util::wildcard::CollectionAny>(view);
}
} // namespace

TEST(CollectionBulkOperationsTest, RemoveAllUsesValueEquality) {
    auto values = listOf("a", "b", "c");
    auto removed = listOf("b", "z");
    EXPECT_TRUE(values->removeAll(anyView(removed)));
    ASSERT_EQ(2, values->size());
    EXPECT_TRUE(values->get(0)->equals(::jxx::NEW<JxxString>("a")));
    EXPECT_TRUE(values->get(1)->equals(::jxx::NEW<JxxString>("c")));
}

TEST(CollectionBulkOperationsTest, RetainAllUsesValueEquality) {
    auto values = listOf("a", "c");
    auto retained = listOf("a", "z");
    EXPECT_TRUE(values->retainAll(anyView(retained)));
    ASSERT_EQ(1, values->size());
    EXPECT_TRUE(values->get(0)->equals(::jxx::NEW<JxxString>("a")));
}
