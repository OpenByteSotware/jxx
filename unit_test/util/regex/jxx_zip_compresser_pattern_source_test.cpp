#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/regex/jxx.util.regex.Pattern.h"
#include "util/regex/jxx.util.regex.Matcher.h"

TEST(ZipCompresserParity, PatternQuoteProducesLiteralPattern) {
    auto expression = ::jxx::NEW<::jxx::lang::String>(
        "[^\\w/\\:](?![\\w].[\\w])");
    const auto quoted = ::jxx::util::regex::Pattern::quote(expression);
    const auto pattern = ::jxx::util::regex::Pattern::compile(quoted);
    EXPECT_TRUE(pattern->matcher(expression)->find());
    EXPECT_FALSE(pattern->matcher(
        ::jxx::NEW<::jxx::lang::String>("../payload"))->find());
}
