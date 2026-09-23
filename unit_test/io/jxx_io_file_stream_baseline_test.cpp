#include <gtest/gtest.h>

#include <cstdio>

#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
#include "io/jxx.io.IOHelper.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace {

::jxx::Ptr<::jxx::lang::String> temporaryPath() {
    return ::jxx::NEW<::jxx::lang::String>(
        "jxx_file_stream_baseline_test.tmp");
}

void removeTemporaryFile(
    const ::jxx::Ptr<::jxx::lang::String>& path) {
    std::remove(path->utf8().c_str());
}

TEST(FileStreamBaselineTest, OverwriteAppendReadSkipAvailableAndEof) {
    const auto path = temporaryPath();
    removeTemporaryFile(path);

    {
        const auto output =
            ::jxx::NEW<::jxx::io::FileOutputStream>(path);
        output->write(1);
        auto bytes = std::make_shared<
            ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(2);
        (*bytes)[0] = 2;
        (*bytes)[1] = 3;
        output->write(bytes, 0, 2);
        output->close();
    }

    {
        const auto output =
            ::jxx::NEW<::jxx::io::FileOutputStream>(path, true);
        output->write(4);
        output->close();
    }

    {
        const auto input =
            ::jxx::NEW<::jxx::io::FileInputStream>(path);
        EXPECT_EQ(4, input->available());
        EXPECT_EQ(1, input->read());
        EXPECT_EQ(2, input->skip(2));
        EXPECT_EQ(4, input->read());
        EXPECT_EQ(-1, input->read());
        input->close();
    }

    removeTemporaryFile(path);
}

TEST(FileStreamBaselineTest, ArrayRangeValidationMatchesIoHelper) {
    const auto path = temporaryPath();
    removeTemporaryFile(path);
    const auto output =
        ::jxx::NEW<::jxx::io::FileOutputStream>(path);
    auto bytes = std::make_shared<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(2);

    EXPECT_THROW(
        output->write(bytes, -1, 1),
        ::jxx::lang::IndexOutOfBoundsException);
    EXPECT_THROW(
        output->write(bytes, 0, 3),
        ::jxx::lang::IndexOutOfBoundsException);

    ::jxx::lang::ByteArray nullBytes;
    EXPECT_THROW(
        output->write(nullBytes, 0, 1),
        ::jxx::lang::NullPointerException);

    output->close();
    removeTemporaryFile(path);
}

} // namespace
