#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.wildcard.CollectionAdapters.h"

TEST(CollectionAnyViewTest, DelegatesErasedMembershipWithoutDowncast) {
    auto values = ::jxx::NEW<::jxx::util::ArrayList<::jxx::lang::String>>();
    values->add(::jxx::NEW<::jxx::lang::String>("b"));
    auto view = ::jxx::NEW<
        ::jxx::util::wildcard::CollectionAnyView<::jxx::lang::String>>(
        ::jxx::CAST<::jxx::util::Collection<::jxx::lang::String>>(values));
    EXPECT_TRUE(view->containsObject(
        ::jxx::CAST<::jxx::lang::Object>(
            ::jxx::NEW<::jxx::lang::String>("b"))));
}
