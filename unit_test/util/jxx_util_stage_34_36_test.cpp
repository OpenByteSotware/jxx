#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Formatter.h"
#include "util/jxx.util.FormatterClosedException.h"
#include "util/jxx.util.InputMismatchException.h"
#include "util/jxx.util.Scanner.h"
#include "util/regex/jxx.util.regex.Matcher.h"
#include "util/regex/jxx.util.regex.Pattern.h"

TEST(FormatterClosedStateTest, AccessorsRejectClosedFormatter) {
    const auto formatter = ::jxx::NEW<::jxx::util::Formatter>();
    formatter->close();
    EXPECT_THROW(formatter->toString(), ::jxx::util::FormatterClosedException);
    EXPECT_THROW(formatter->locale(), ::jxx::util::FormatterClosedException);
    EXPECT_THROW(formatter->flush(), ::jxx::util::FormatterClosedException);
}

TEST(RegexRegionTest, MatchesAndLookingAtUseConfiguredRegion) {
    const auto pattern = ::jxx::util::regex::Pattern::compile(
        ::jxx::NEW<::jxx::lang::String>("[0-9]+"));
    auto matcher = pattern->matcher(::jxx::NEW<::jxx::lang::String>("abc123xyz"));
    matcher->region(3, 6);
    EXPECT_TRUE(matcher->matches());
    EXPECT_EQ(3, matcher->start());
    EXPECT_EQ(6, matcher->end());
    matcher->region(3, 9);
    EXPECT_TRUE(matcher->lookingAt());
    EXPECT_EQ(3, matcher->start());
    EXPECT_EQ(6, matcher->end());
}

TEST(ScannerNumericStateTest, InputMismatchDoesNotConsumeToken) {
    const auto scanner = ::jxx::NEW<::jxx::util::Scanner>(
        ::jxx::NEW<::jxx::lang::String>("bad 42"));
    EXPECT_THROW(scanner->nextInt(), ::jxx::util::InputMismatchException);
    EXPECT_EQ("bad", scanner->next()->utf8());
    EXPECT_EQ(42, scanner->nextInt());
}
