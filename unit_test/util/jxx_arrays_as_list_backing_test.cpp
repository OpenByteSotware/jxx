#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "util/jxx.util.Arrays.h"

TEST(ArraysAsListParity, IsFixedSizeAndBackedBySuppliedArray)
{
    ::jxx::initialize();
    using String = ::jxx::lang::String;
    auto array = ::jxx::NEW<::jxx::lang::JxxArray<
        ::jxx::Ptr<String>, 1U>>(2);
    (*array)[0] = ::jxx::NEW<String>("first");
    (*array)[1] = ::jxx::NEW<String>("second");
    auto list = ::jxx::util::Arrays::asList<String>(array);

    ASSERT_EQ(list->size(), 2);
    EXPECT_EQ(list->get(0)->utf8(), "first");
    auto old = list->set(0, ::jxx::NEW<String>("changed"));
    ASSERT_NE(old, nullptr);
    EXPECT_EQ(old->utf8(), "first");
    EXPECT_EQ((*array)[0]->utf8(), "changed");

    (*array)[1] = ::jxx::NEW<String>("array-change");
    EXPECT_EQ(list->get(1)->utf8(), "array-change");
}

TEST(ArraysAsListParity, RejectsStructuralModification)
{
    ::jxx::initialize();
    auto list = ::jxx::util::Arrays::asList<::jxx::lang::Object>(
        ::jxx::NEW<::jxx::lang::Object>());
    EXPECT_THROW(list->add(::jxx::NEW<::jxx::lang::Object>()),
        ::jxx::lang::UnsupportedOperationException);
    EXPECT_THROW(list->remove(0),
        ::jxx::lang::UnsupportedOperationException);
    EXPECT_THROW(list->clear(),
        ::jxx::lang::UnsupportedOperationException);
}
