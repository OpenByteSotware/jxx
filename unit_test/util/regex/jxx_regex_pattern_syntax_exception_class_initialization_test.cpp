#include <gtest/gtest.h>

#include "util/regex/jxx.util.regex.ClassInitialization.h"
#include "util/regex/jxx.util.regex.PatternSyntaxException.h"

TEST(JxxRegexClassInitialization, PatternSyntaxExceptionHasClassMetadata)
{
    ::jxx::util::regex::initializeClasses();

    EXPECT_NE(
        ::jxx::util::regex::PatternSyntaxException::JxxClassInfoMarker::Class(),
        nullptr);
}

TEST(JxxRegexClassInitialization, PatternSyntaxExceptionInitializationIsIdempotent)
{
    const auto before =
        ::jxx::util::regex::PatternSyntaxException::JxxClassInfoMarker::Class();

    EXPECT_NO_THROW(::jxx::util::regex::initializeClasses());
    EXPECT_NO_THROW(::jxx::util::regex::initializeClasses());

    EXPECT_EQ(
        before,
        ::jxx::util::regex::PatternSyntaxException::JxxClassInfoMarker::Class());
}
