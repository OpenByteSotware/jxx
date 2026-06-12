#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.ResourceBundle.h"
#include "tests/TestHelpers.h"

namespace {
using namespace jxx::tests;
using jxx::lang::String;
using jxx::util::Locale;
using jxx::util::ResourceBundle;

TEST(ResourceBundleTests, ReturnsFrenchBundleWhenRequested) {
    auto bundle = ResourceBundle::getBundle(String::valueOf("messages"), Locale::FRANCE);
    ASSERT_TRUE(bundle != nullptr);
    ExpectStringEq(bundle->getString(String::valueOf("hello")), "Bonjour");
}

TEST(ResourceBundleTests, FallsBackToEnglishBundle) {
    auto ja = Locale::forLanguageTag(String::valueOf("ja-JP"));
    auto bundle = ResourceBundle::getBundle(String::valueOf("messages"), ja);
    ASSERT_TRUE(bundle != nullptr);
    ExpectStringEq(bundle->getString(String::valueOf("bye")), "Goodbye");
}

TEST(ResourceBundleTests, UnknownBaseNameReturnsNull) {
    auto bundle = ResourceBundle::getBundle(String::valueOf("other"), Locale::US);
    EXPECT_EQ(bundle, nullptr);
}

} // namespace
