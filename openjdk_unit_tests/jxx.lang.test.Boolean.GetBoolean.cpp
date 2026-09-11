#include <gtest/gtest.h>

#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.String.h"

namespace {

/*
 * Clean-room GoogleTest adaptation of the observable behavior exercised by:
 *   openjdk/jdk8/jdk/test/java/lang/Boolean/GetBoolean.java
 * Upstream bug: 4252308
 * Upstream summary: test Boolean.getBoolean method with empty key
 *
 * This file does not copy the upstream implementation. It preserves the test
 * inputs and expected no-throw behavior. Define JXX_HAS_BOOLEAN_GET_BOOLEAN
 * once jxx::lang::Boolean::getBoolean(const jxx::Ptr<String>&) exists.
 */
TEST(JxxLangBooleanOpenJdk8, GetBooleanAcceptsEmptyPropertyName)
{
#if defined(JXX_HAS_BOOLEAN_GET_BOOLEAN)
    const auto empty = ::jxx::NEW<::jxx::lang::String>("");
    EXPECT_NO_THROW((void)::jxx::lang::Boolean::getBoolean(empty));
#else
    GTEST_SKIP() << "JXX missing Boolean::getBoolean(String); OpenJDK 8 test is cataloged as BLOCKED_MISSING_API";
#endif
}

TEST(JxxLangBooleanOpenJdk8, GetBooleanAcceptsNullPropertyName)
{
#if defined(JXX_HAS_BOOLEAN_GET_BOOLEAN)
    const ::jxx::Ptr<::jxx::lang::String> name = nullptr;
    EXPECT_NO_THROW((void)::jxx::lang::Boolean::getBoolean(name));
#else
    GTEST_SKIP() << "JXX missing Boolean::getBoolean(String); OpenJDK 8 test is cataloged as BLOCKED_MISSING_API";
#endif
}

} // namespace
