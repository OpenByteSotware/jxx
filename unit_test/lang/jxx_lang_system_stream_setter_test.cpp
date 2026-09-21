#include <gtest/gtest.h>
#include "lang/jxx.lang.System.h"

namespace {
using ::jxx::lang::System;

TEST(SystemStreamParityTest, SettersReplaceStandardStreams) {
    const auto originalIn = System::in;
    const auto originalOut = System::out;
    const auto originalErr = System::err;

    System::setIn(originalIn);
    System::setOut(originalOut);
    System::setErr(originalErr);

    EXPECT_EQ(originalIn.get(), System::in.get());
    EXPECT_EQ(originalOut.get(), System::out.get());
    EXPECT_EQ(originalErr.get(), System::err.get());
}

TEST(SystemStreamParityTest, SettersAcceptNullReferences) {
    const auto originalIn = System::in;
    const auto originalOut = System::out;
    const auto originalErr = System::err;
    const ::jxx::Ptr<::jxx::io::InputStream> nullIn;
    const ::jxx::Ptr<::jxx::io::PrintStream> nullPrint;

    System::setIn(nullIn);
    System::setOut(nullPrint);
    System::setErr(nullPrint);
    EXPECT_EQ(nullptr, System::in);
    EXPECT_EQ(nullptr, System::out);
    EXPECT_EQ(nullptr, System::err);

    System::setIn(originalIn);
    System::setOut(originalOut);
    System::setErr(originalErr);
}
} // namespace
