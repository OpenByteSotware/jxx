#include <gtest/gtest.h>
#include <type_traits>

#include "util/jxx.util.ListResourceBundle.h"

TEST(JxxListResourceBundleHierarchyTest, IsAbstractResourceBundleSubclass) {
    static_assert(std::is_abstract_v<::jxx::util::ListResourceBundle>);
    static_assert(std::is_base_of_v<::jxx::util::ResourceBundle,
                                    ::jxx::util::ListResourceBundle>);
    EXPECT_TRUE(::jxx::util::ResourceBundle::Class()->isAssignableFrom(
        ::jxx::util::ListResourceBundle::Class()));
}
