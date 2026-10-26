#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Scanner.h"

namespace {

TEST(ScannerBigNumberTest, ReadsBigIntegerUsingDefaultRadix) {
    const auto scanner = ::jxx::NEW<::jxx::util::Scanner>(
        ::jxx::NEW<::jxx::lang::String>("123456789012345678901234567890"));
    ASSERT_TRUE(scanner->hasNextBigInteger());
    EXPECT_TRUE(scanner->nextBigInteger()->toString()->equals(
        ::jxx::NEW<::jxx::lang::String>("123456789012345678901234567890")));
}

TEST(ScannerBigNumberTest, ReadsBigIntegerUsingExplicitRadix) {
    const auto scanner = ::jxx::NEW<::jxx::util::Scanner>(
        ::jxx::NEW<::jxx::lang::String>("7fffffffffffffff"));
    ASSERT_TRUE(scanner->hasNextBigInteger(16));
    EXPECT_TRUE(scanner->nextBigInteger(16)->toString()->equals(
        ::jxx::NEW<::jxx::lang::String>("9223372036854775807")));
}

TEST(ScannerBigNumberTest, ReadsBigDecimal) {
    const auto scanner = ::jxx::NEW<::jxx::util::Scanner>(
        ::jxx::NEW<::jxx::lang::String>("12345678901234567890.125"));
    ASSERT_TRUE(scanner->hasNextBigDecimal());
    EXPECT_TRUE(scanner->nextBigDecimal()->toPlainString()->equals(
        ::jxx::NEW<::jxx::lang::String>("12345678901234567890.125")));
}

TEST(ScannerBigNumberTest, HasNextDoesNotConsumeInvalidToken) {
    const auto scanner = ::jxx::NEW<::jxx::util::Scanner>(
        ::jxx::NEW<::jxx::lang::String>("not-a-number 42"));
    EXPECT_FALSE(scanner->hasNextBigInteger());
    EXPECT_TRUE(scanner->next()->equals(
        ::jxx::NEW<::jxx::lang::String>("not-a-number")));
    EXPECT_TRUE(scanner->hasNextBigInteger());
}

} // namespace
