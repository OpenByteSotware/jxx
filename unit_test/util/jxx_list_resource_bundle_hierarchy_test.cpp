#include <gtest/gtest.h>
#include <type_traits>

#include "util/jxx.util.ListResourceBundle.h"

TEST(JxxListResourceBundleHierarchyTest, IsAbstractResourceBundleSubclass)
{
    static_assert(std::is_abstract_v<::jxx::util::ListResourceBundle>);
    static_assert(std::is_base_of_v<
        ::jxx::util::ResourceBundle,
        ::jxx::util::ListResourceBundle>);

    const auto resourceBundleClass =
        ::jxx::util::ResourceBundle::Class();
    const auto listResourceBundleClass =
        ::jxx::util::ListResourceBundle::Class();

    ASSERT_NE(nullptr, resourceBundleClass);
    ASSERT_NE(nullptr, listResourceBundleClass);
    EXPECT_NE(resourceBundleClass, listResourceBundleClass);
    EXPECT_TRUE(resourceBundleClass->isAssignableFrom(
        listResourceBundleClass));
}
