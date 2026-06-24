#include <memory>
#include <string>

#include <gtest/gtest.h>
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.InputMismatchException.h"
#include "util/jxx.util.Scanner.h"
#include "util/regex/jxx.util.regex.Pattern.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace {

using jxx::Ptr;
using jxx::lang::String;
using jxx::util::InputMismatchException;
using jxx::util::Scanner;
using jxx::util::regex::Pattern;

static Ptr<String> S(const char* s) {
    return std::make_shared<String>(s);
}

class ScannerStage4Test : public ::testing::Test {
protected:
    void ExpectStringEq(const Ptr<String>& actual, const char* expected) {
        ASSERT_NE(actual, nullptr);
        EXPECT_EQ(actual->utf8(), std::string(expected));
    }
};

TEST_F(ScannerStage4Test, HasNextByteAndNextByteWork) {
    auto scanner = std::make_shared<Scanner>(S("127 -128 255"));
    EXPECT_TRUE(scanner->hasNextByte());
    EXPECT_EQ(scanner->nextByte(), 127);
    EXPECT_TRUE(scanner->hasNextByte());
    EXPECT_EQ(scanner->nextByte(), -128);
    EXPECT_FALSE(scanner->hasNextByte());
    EXPECT_THROW(scanner->nextByte(), InputMismatchException);
}

TEST_F(ScannerStage4Test, HasNextByteWithRadixWorks) {
    auto scanner = std::make_shared<Scanner>(S("7F 80"));
    EXPECT_TRUE(scanner->hasNextByte(16));
    EXPECT_EQ(scanner->nextByte(16), 127);
    EXPECT_FALSE(scanner->hasNextByte(16));
}

TEST_F(ScannerStage4Test, HasNextShortAndNextShortWork) {
    auto scanner = std::make_shared<Scanner>(S("32767 -32768 40000"));
    EXPECT_TRUE(scanner->hasNextShort());
    EXPECT_EQ(scanner->nextShort(), 32767);
    EXPECT_TRUE(scanner->hasNextShort());
    EXPECT_EQ(scanner->nextShort(), -32768);
    EXPECT_FALSE(scanner->hasNextShort());
    EXPECT_THROW(scanner->nextShort(), InputMismatchException);
}

TEST_F(ScannerStage4Test, HasNextFloatAndNextFloatWork) {
    auto scanner = std::make_shared<Scanner>(S("3.5 -2.25 oops"));
    EXPECT_TRUE(scanner->hasNextFloat());
    EXPECT_FLOAT_EQ(scanner->nextFloat(), 3.5f);
    EXPECT_TRUE(scanner->hasNextFloat());
    EXPECT_FLOAT_EQ(scanner->nextFloat(), -2.25f);
    EXPECT_FALSE(scanner->hasNextFloat());
    EXPECT_THROW(scanner->nextFloat(), InputMismatchException);
}

TEST_F(ScannerStage4Test, Stage3PatternTokenMethodsStillWork) {
    auto scanner = std::make_shared<Scanner>(S("123 abc"));
    EXPECT_TRUE(scanner->hasNext(S("\\d+")));
    ExpectStringEq(scanner->next(S("\\d+")), "123");
    auto mr = scanner->match();
    ASSERT_NE(mr, nullptr);
    EXPECT_EQ(mr->group()->utf8(), "123");
    EXPECT_TRUE(scanner->hasNext(S("[a-z]+")));
    ExpectStringEq(scanner->next(S("[a-z]+")), "abc");
}

TEST_F(ScannerStage4Test, Stage2RegexMethodsStillWork) {
    auto scanner = std::make_shared<Scanner>(S("abc 123\nxyz"));
    ExpectStringEq(scanner->findInLine(S("\\d+")), "123");
    ExpectStringEq(scanner->nextLine(), "");
    ExpectStringEq(scanner->findWithinHorizon(S("xyz"), 0), "xyz");
}

TEST_F(ScannerStage4Test, InvalidRadixStillThrowsIllegalArgumentException) {
    auto scanner = std::make_shared<Scanner>(S("1"));
    EXPECT_THROW(scanner->hasNextByte(1), jxx::lang::IllegalArgumentException);
    EXPECT_THROW(scanner->nextShort(37), jxx::lang::IllegalArgumentException);
}

TEST_F(ScannerStage4Test, NullPatternArgumentsStillThrowNullPointerException) {
    auto scanner = std::make_shared<Scanner>(S("abc"));
    EXPECT_THROW((void)scanner->hasNext(Ptr<String>()), jxx::lang::NullPointerException);
    EXPECT_THROW((void)scanner->next(Ptr<Pattern>()), jxx::lang::NullPointerException);
}

} // anonymous namespace
