#include <gtest/gtest.h>
#include <type_traits>

#include "util/jxx.util.Locale.h"
#include "lang/jxx.lang.String.h"

TEST(JxxLocaleJava8HierarchyTest, IsFinalCloneableAndSerializable) {
    static_assert(std::is_final_v<::jxx::util::Locale>);
    EXPECT_TRUE(::jxx::lang::Cloneable::Class()->isAssignableFrom(
        ::jxx::util::Locale::Class()));
    EXPECT_TRUE(::jxx::io::SerializableI::Class()->isAssignableFrom(
        ::jxx::util::Locale::Class()));
}

TEST(JxxLocaleJava8HierarchyTest, CloneIsDistinctButEqual) {
    const auto locale = ::jxx::NEW<::jxx::util::Locale>(
        ::jxx::NEW<::jxx::lang::String>("en"),
        ::jxx::NEW<::jxx::lang::String>("US"));
    const auto clone = ::jxx::CAST<::jxx::util::Locale>(locale->clone());
    ASSERT_NE(clone, nullptr);
    EXPECT_NE(clone, locale);
    EXPECT_TRUE(locale->equals(clone));
    EXPECT_EQ(locale->hashCode(), clone->hashCode());
}
