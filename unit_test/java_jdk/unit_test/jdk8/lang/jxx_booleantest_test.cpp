#include <gtest/gtest.h>
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Finally.h"
#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "util/jxx.util.Objects.h"

namespace jxx::jdk8::lang {
    TEST(BooleanTest, ConstructorsAndPrimitiveValue) {
        EXPECT_TRUE((::jxx::NEW<::jxx::lang::Boolean>(true))->booleanValue());
        EXPECT_FALSE((::jxx::NEW<::jxx::lang::Boolean>(false))->booleanValue());
        EXPECT_TRUE((::jxx::NEW<::jxx::lang::Boolean>(::jxx::NEW<::jxx::lang::String>("TrUe")))->booleanValue());
        EXPECT_FALSE((::jxx::NEW<::jxx::lang::Boolean>(::jxx::NEW<::jxx::lang::String>("yes")))->booleanValue());
        EXPECT_FALSE((::jxx::NEW<::jxx::lang::Boolean>(::jxx::Ptr<::jxx::lang::String>{}))->booleanValue());
    }

    TEST(BooleanTest, ConstantsHaveDocumentedValues) {
        EXPECT_EQ(::jxx::lang::Boolean::TRUE_.get(), ::jxx::lang::Boolean::valueOf(true).get());
        EXPECT_EQ(::jxx::lang::Boolean::FALSE_.get(), ::jxx::lang::Boolean::valueOf(false).get());
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Boolean::TYPE, ::jxx::lang::Boolean::TYPE));
    }

    TEST(BooleanTest, ParseBooleanRecognizesOnlyTrueIgnoringCase) {
        EXPECT_TRUE(::jxx::lang::Boolean::parseBoolean(::jxx::NEW<::jxx::lang::String>("true")));
        EXPECT_TRUE(::jxx::lang::Boolean::parseBoolean(::jxx::NEW<::jxx::lang::String>("TRUE")));
        EXPECT_FALSE(::jxx::lang::Boolean::parseBoolean(::jxx::NEW<::jxx::lang::String>("false")));
        EXPECT_FALSE(::jxx::lang::Boolean::parseBoolean(::jxx::NEW<::jxx::lang::String>("1")));
        EXPECT_FALSE(::jxx::lang::Boolean::parseBoolean(nullptr));
    }

    TEST(BooleanTest, ValueOfUsesCanonicalInstances) {
        EXPECT_EQ(::jxx::lang::Boolean::TRUE_.get(), ::jxx::lang::Boolean::valueOf(::jxx::NEW<::jxx::lang::String>("true")).get());
        EXPECT_EQ(::jxx::lang::Boolean::TRUE_.get(), ::jxx::lang::Boolean::valueOf(::jxx::NEW<::jxx::lang::String>("TrUe")).get());
        EXPECT_EQ(::jxx::lang::Boolean::FALSE_.get(), ::jxx::lang::Boolean::valueOf(::jxx::NEW<::jxx::lang::String>("anything")).get());
        EXPECT_EQ(::jxx::lang::Boolean::FALSE_.get(), ::jxx::lang::Boolean::valueOf(::jxx::Ptr<::jxx::lang::String>{}).get());
    }

    TEST(BooleanTest, StringConversionsUseLowercaseWords) {
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("true"), ::jxx::lang::String::valueOf(::jxx::lang::Boolean::toString(true))));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("false"), ::jxx::lang::String::valueOf(::jxx::lang::Boolean::toString(false))));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("true"), ::jxx::lang::String::valueOf(::jxx::lang::Boolean::TRUE_->toString())));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("false"), ::jxx::lang::String::valueOf(::jxx::lang::Boolean::FALSE_->toString())));
    }

    TEST(BooleanTest, EqualityHashAndComparisonFollowBooleanOrdering) {
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Boolean::TRUE_, ::jxx::NEW<::jxx::lang::Boolean>(true)));
        EXPECT_FALSE(::jxx::util::Objects::equals(::jxx::lang::Boolean::TRUE_, ::jxx::lang::Boolean::FALSE_));
        EXPECT_EQ(1231, ::jxx::lang::Boolean::TRUE_->hashCode());
        EXPECT_EQ(1237, ::jxx::lang::Boolean::FALSE_->hashCode());
        EXPECT_EQ(1231, ::jxx::lang::Boolean::hashCode(true));
        EXPECT_EQ(1237, ::jxx::lang::Boolean::hashCode(false));
        EXPECT_TRUE((::jxx::lang::Boolean::FALSE_->compareTo(::jxx::lang::Boolean::TRUE_) < 0));
        EXPECT_TRUE((::jxx::lang::Boolean::compare(false, true) < 0));
        EXPECT_EQ(0, ::jxx::lang::Boolean::compare(true, true));
    }

    TEST(BooleanTest, LogicalOperationsMatchTruthTables) {
        EXPECT_FALSE(::jxx::lang::Boolean::logicalAnd(true, false));
        EXPECT_TRUE(::jxx::lang::Boolean::logicalAnd(true, true));
        EXPECT_TRUE(::jxx::lang::Boolean::logicalOr(false, true));
        EXPECT_FALSE(::jxx::lang::Boolean::logicalOr(false, false));
        EXPECT_TRUE(::jxx::lang::Boolean::logicalXor(true, false));
        EXPECT_FALSE(::jxx::lang::Boolean::logicalXor(true, true));
    }

    TEST(BooleanTest, GetBooleanReadsSystemProperty) {
        ::jxx::Ptr<::jxx::lang::String> key = ::jxx::CONVERT<::jxx::Ptr<::jxx::lang::String>>("jdk8.lang.BooleanTest.flag");
        ::jxx::Ptr<::jxx::lang::String> previous = ::jxx::lang::System::getProperty(key);
        {
            auto jxxFinally0 = ::jxx::lang::makeFinally([&]() {
                if ((previous == nullptr)) {
                    ::jxx::lang::System::clearProperty(key);
                } else {
                    ::jxx::lang::System::setProperty(key, previous);
                }
            });
            try {
                ::jxx::lang::System::setProperty(key, ::jxx::NEW<::jxx::lang::String>("TrUe"));
                EXPECT_TRUE(::jxx::lang::Boolean::getBoolean(key));
                ::jxx::lang::System::setProperty(key, ::jxx::NEW<::jxx::lang::String>("false"));
                EXPECT_FALSE(::jxx::lang::Boolean::getBoolean(key));
                ::jxx::lang::System::clearProperty(key);
                EXPECT_FALSE(::jxx::lang::Boolean::getBoolean(key));
            } catch (...) {
                jxxFinally0.runAndDismiss();
                throw;
            }
        }
    }

} // namespace jxx::jdk8::lang
