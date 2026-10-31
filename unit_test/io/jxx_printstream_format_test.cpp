#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.PrintStream.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Locale.h"

namespace {

TEST(PrintStreamFormatTest, PrintfFormatsArgumentsAndReturnsThisStream) {
    const auto output = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto stream = ::jxx::NEW<::jxx::io::PrintStream>(output);
    const auto args = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Object>, 1U>>(1);
    (*args)[0] = ::jxx::lang::Integer::valueOf(42);

    const auto result = stream->printf(
        ::jxx::NEW<::jxx::lang::String>("value=%d"), args);

    EXPECT_EQ(stream, result);
    EXPECT_TRUE(output->toString()->equals(
        ::jxx::NEW<::jxx::lang::String>("value=42")));
}

TEST(PrintStreamFormatTest, LocaleOverloadDelegatesToFormatter) {
    const auto output = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto stream = ::jxx::NEW<::jxx::io::PrintStream>(output);
    const auto args = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Object>, 1U>>(0);

    EXPECT_EQ(stream, stream->format(
        ::jxx::util::Locale::US,
        ::jxx::NEW<::jxx::lang::String>("ok%n"), args));
}

} // namespace
