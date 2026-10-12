#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "util/regex/jxx.util.regex.MatchResult.h"
#include "util/regex/jxx.util.regex.Matcher.h"
#include "util/regex/jxx.util.regex.Pattern.h"

TEST(JxxRegexClassInitialization, PublicTypesHaveClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(::jxx::util::regex::MatchResult::Class(), nullptr);
    EXPECT_NE(::jxx::util::regex::Matcher::Class(), nullptr);
    EXPECT_NE(::jxx::util::regex::Pattern::Class(), nullptr);
}
