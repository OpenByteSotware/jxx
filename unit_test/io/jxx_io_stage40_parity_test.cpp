#include <gtest/gtest.h>

#include <cstdio>

#include "io/jxx.io.FileDescriptor.h"
#include "io/jxx.io.FileWriter.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.RuntimeException.h"
#include "lang/jxx.lang.String.h"

TEST(IOExceptionParity, PreservesMessageAndCause) {
    const auto cause = ::jxx::NEW<::jxx::lang::RuntimeException>("cause");
    const auto message = ::jxx::NEW<::jxx::lang::String>("write failed");
    const auto exception = ::jxx::NEW<::jxx::io::IOException>(message, cause);

    EXPECT_EQ(cause.get(), exception->getCause().get());
    ASSERT_NE(nullptr, exception->getMessage());
    EXPECT_TRUE(exception->getMessage()->equals(message));
}

TEST(IOExceptionParity, CauseOnlyConstructorPreservesCause) {
    const auto cause = ::jxx::NEW<::jxx::lang::RuntimeException>("cause");
    const auto exception = ::jxx::NEW<::jxx::io::IOException>(cause);

    EXPECT_EQ(cause.get(), exception->getCause().get());
    ASSERT_NE(nullptr, exception->getMessage());
}

TEST(FileWriterParity, ConstructsFromFileDescriptor) {
    FILE* const handle = std::tmpfile();
    ASSERT_NE(nullptr, handle);
    const auto descriptor = ::jxx::NEW<::jxx::io::FileDescriptor>(
        handle, false);

    EXPECT_NO_THROW({
        const auto writer = ::jxx::NEW<::jxx::io::FileWriter>(descriptor);
        ASSERT_NE(nullptr, writer);
        writer->flush();
    });

    EXPECT_FALSE(descriptor->valid());
}
