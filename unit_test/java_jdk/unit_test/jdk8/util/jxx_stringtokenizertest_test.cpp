#include <gtest/gtest.h>
#include <type_traits>
#include <utility>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.System.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.Objects.h"
#include "util/jxx.util.StringTokenizer.h"

namespace jxx::jdk8::util {
    TEST(StringTokenizerTest, DefaultWhitespaceTokenizationWorks) {
        ::jxx::Ptr<::jxx::util::StringTokenizer> t = ::jxx::NEW<::jxx::util::StringTokenizer>(::jxx::NEW<::jxx::lang::String>(" one\ttwo\nthree "));
        EXPECT_EQ(3, t->countTokens());
        EXPECT_TRUE(t->hasMoreTokens());
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("one"), t->nextToken()));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("two"), t->nextToken()));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("three"), t->nextElement()));
        EXPECT_FALSE(t->hasMoreElements());
    }

    TEST(StringTokenizerTest, CustomDelimitersCanBeReturned) {
        ::jxx::Ptr<::jxx::util::StringTokenizer> t = ::jxx::NEW<::jxx::util::StringTokenizer>(::jxx::NEW<::jxx::lang::String>("a,b;c"), ::jxx::NEW<::jxx::lang::String>(",;"), true);
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("a"), t->nextToken()));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>(","), t->nextToken()));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("b"), t->nextToken()));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>(";"), t->nextToken()));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("c"), t->nextToken()));
    }

    TEST(StringTokenizerTest, NextTokenMayChangeDelimiters) {
        ::jxx::Ptr<::jxx::util::StringTokenizer> t = ::jxx::NEW<::jxx::util::StringTokenizer>(::jxx::NEW<::jxx::lang::String>("a,b;c"), ::jxx::NEW<::jxx::lang::String>(","));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("a"), t->nextToken()));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>(",b"), t->nextToken(::jxx::NEW<::jxx::lang::String>(";"))));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("c"), t->nextToken()));
    }

    TEST(StringTokenizerTest, ExhaustionThrows) {
        EXPECT_THROW(
            {
                ::jxx::Ptr<::jxx::util::StringTokenizer> t = ::jxx::NEW<::jxx::util::StringTokenizer>(::jxx::NEW<::jxx::lang::String>(""));
                t->nextToken();
            },
            ::jxx::util::NoSuchElementException);
    }

} // namespace jxx::jdk8::util
