#include <gtest/gtest.h>
#include <type_traits>
#include "util/regex/jxx.util.regex.Matcher.h"

TEST(JxxMatcherHierarchyTest, MatchesObjectAndMatchResultHierarchy) {
    static_assert(std::is_final_v<::jxx::util::regex::Matcher>);
    static_assert(std::is_base_of_v<::jxx::lang::Object,
                                    ::jxx::util::regex::Matcher>);
    static_assert(std::is_base_of_v<::jxx::util::regex::MatchResult,
                                    ::jxx::util::regex::Matcher>);
    EXPECT_TRUE(::jxx::util::regex::MatchResult::Class()->isAssignableFrom(
        ::jxx::util::regex::Matcher::Class()));
}
