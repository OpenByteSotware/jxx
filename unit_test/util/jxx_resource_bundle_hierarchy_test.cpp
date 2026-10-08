#include <gtest/gtest.h>
#include <type_traits>

#include "util/jxx.util.ResourceBundle.h"

TEST(JxxResourceBundleHierarchyTest, IsAbstractObjectClassWithMetadata) {
    static_assert(std::is_abstract_v<::jxx::util::ResourceBundle>);
    static_assert(std::is_base_of_v<::jxx::lang::Object,
                                    ::jxx::util::ResourceBundle>);
    EXPECT_NE(::jxx::util::ResourceBundle::Class(), nullptr);
}
