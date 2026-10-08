#include <gtest/gtest.h>
#include <type_traits>

#include "util/jxx.util.PropertyResourceBundle.h"

TEST(JxxPropertyResourceBundleHierarchyTest, IsConcreteResourceBundleSubclass) {
    static_assert(!std::is_abstract_v<::jxx::util::PropertyResourceBundle>);
    static_assert(std::is_base_of_v<::jxx::util::ResourceBundle,
                                    ::jxx::util::PropertyResourceBundle>);
    EXPECT_TRUE(::jxx::util::ResourceBundle::Class()->isAssignableFrom(
        ::jxx::util::PropertyResourceBundle::Class()));
}

TEST(JxxPropertyResourceBundleHierarchyTest, NullSourcesAreRejected) {
    EXPECT_THROW(
        (void)::jxx::NEW<::jxx::util::PropertyResourceBundle>(
            ::jxx::Ptr<::jxx::io::InputStream>()),
        ::jxx::lang::NullPointerException);
    EXPECT_THROW(
        (void)::jxx::NEW<::jxx::util::PropertyResourceBundle>(
            ::jxx::Ptr<::jxx::io::Reader>()),
        ::jxx::lang::NullPointerException);
}
