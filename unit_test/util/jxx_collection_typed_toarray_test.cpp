#include <gtest/gtest.h>

#include "lang/jxx.lang.ArrayStoreException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ArrayList.h"

TEST(CollectionTypedToArrayTest, ReusesLargeEnoughDestination)
{
    auto values = ::jxx::NEW<::jxx::util::ArrayList<::jxx::lang::String>>();
    values->add(::jxx::NEW<::jxx::lang::String>("a"));
    values->add(::jxx::NEW<::jxx::lang::String>("b"));

    auto destination = ::jxx::NEW<::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>>(3);
    (*destination)[2] = ::jxx::NEW<::jxx::lang::String>("stale");

    const auto result = values->toArray(destination);
    EXPECT_EQ(destination.get(), result.get());
    EXPECT_TRUE((*result)[0]->equals(::jxx::NEW<::jxx::lang::String>("a")));
    EXPECT_TRUE((*result)[1]->equals(::jxx::NEW<::jxx::lang::String>("b")));
    EXPECT_EQ(nullptr, (*result)[2]);
}

TEST(CollectionTypedToArrayTest, AllocatesWhenDestinationIsTooSmall)
{
    auto values = ::jxx::NEW<::jxx::util::ArrayList<::jxx::lang::String>>();
    values->add(::jxx::NEW<::jxx::lang::String>("a"));
    values->add(::jxx::NEW<::jxx::lang::String>("b"));

    auto destination = ::jxx::NEW<::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>>(0);
    const auto result = values->toArray(destination);

    EXPECT_NE(destination.get(), result.get());
    EXPECT_EQ(2, result->length);
}

TEST(CollectionTypedToArrayTest, RejectsNullDestination)
{
    auto values = ::jxx::NEW<::jxx::util::ArrayList<::jxx::lang::String>>();
    ::jxx::Ptr<::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>> destination;
    EXPECT_THROW(values->toArray(destination), ::jxx::lang::NullPointerException);
}
