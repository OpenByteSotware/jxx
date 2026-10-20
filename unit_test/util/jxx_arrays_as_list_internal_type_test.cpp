#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.List.h"

TEST(ArraysAsListParity, PublicApiReturnsOnlyListInterface)
{
    ::jxx::initialize();
    auto result = ::jxx::util::Arrays::asList<::jxx::lang::Object>(
        ::jxx::CAST<::jxx::lang::Object>(
            ::jxx::NEW<::jxx::lang::String>("button")));
    ::jxx::Ptr<::jxx::util::List<::jxx::lang::Object>> publicView = result;
    ASSERT_NE(publicView, nullptr);
    EXPECT_EQ(publicView->size(), 1);
}
