#include <gtest/gtest.h>

#include "lang/jxx.lang.System.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

using namespace jxx::lang;

TEST(SystemEnvParity, NullAndEmpty)
{
    ::jxx::Ptr<String> none;

    EXPECT_THROW(
        System::getenv(none),
        NullPointerException);

    const auto emptyName =
        ::jxx::NEW<String>("");

    EXPECT_THROW(
        System::getenv(emptyName),
        IllegalArgumentException);
}

TEST(SystemUtilParity, Identity)
{
    EXPECT_EQ(
        0,
        System::identityHashCode(nullptr));

    auto object =
        ::jxx::NEW<Object>();

    EXPECT_EQ(
        System::identityHashCode(object),
        System::identityHashCode(object));
}

TEST(SystemUtilParity, TimeAndSeparator)
{
    const auto millis1 =
        System::currentTimeMillis();

    const auto millis2 =
        System::currentTimeMillis();

    EXPECT_TRUE(millis2 >= millis1);

    const auto nanos1 =
        System::nanoTime();

    const auto nanos2 =
        System::nanoTime();

    EXPECT_TRUE(nanos2 >= nanos1);

    const auto separator =
        System::lineSeparator();

    ASSERT_NE(nullptr, separator);

#ifdef _WIN32
    EXPECT_EQ(
        std::string("\r\n"),
        separator->utf8());
#else
    EXPECT_EQ(
        std::string("\n"),
        separator->utf8());
#endif
}

TEST(SystemEnvParity, MissingNameReturnsNull)
{
    const auto name =
        ::jxx::NEW<String>(
            "JXX_ENVIRONMENT_VARIABLE_SHOULD_NOT_EXIST_12345");

    EXPECT_EQ(
        nullptr,
        System::getenv(name));
}
