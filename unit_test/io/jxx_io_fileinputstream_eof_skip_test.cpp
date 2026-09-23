#include <gtest/gtest.h>

#include <cstdio>

#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.String.h"

namespace {

::jxx::Ptr<::jxx::lang::String> inputPath() {
    return ::jxx::NEW<::jxx::lang::String>(
        "jxx_fileinputstream_eof_skip_test.tmp");
}

TEST(FileInputStreamEofSkipTest, DistinguishesDataZeroLengthReadAndEof) {
    const auto path = inputPath();
    std::remove(path->utf8().c_str());
    {
        const auto output = ::jxx::NEW<::jxx::io::FileOutputStream>(path);
        output->write(0xFF);
        output->close();
    }

    const auto input = ::jxx::NEW<::jxx::io::FileInputStream>(path);
    auto bytes = std::make_shared<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(1);
    EXPECT_EQ(0, input->read(bytes, 0, 0));
    EXPECT_EQ(255, input->read());
    EXPECT_EQ(-1, input->read());
    EXPECT_EQ(-1, input->read(bytes, 0, 1));
    input->close();
    std::remove(path->utf8().c_str());
}

TEST(FileInputStreamEofSkipTest, NonPositiveSkipDoesNotMoveBackward) {
    const auto path = inputPath();
    std::remove(path->utf8().c_str());
    {
        const auto output = ::jxx::NEW<::jxx::io::FileOutputStream>(path);
        output->write(1);
        output->write(2);
        output->close();
    }

    const auto input = ::jxx::NEW<::jxx::io::FileInputStream>(path);
    EXPECT_EQ(1, input->read());
    EXPECT_EQ(0, input->skip(0));
    EXPECT_EQ(0, input->skip(-1));
    EXPECT_EQ(2, input->read());
    input->close();
    std::remove(path->utf8().c_str());
}

TEST(FileInputStreamEofSkipTest, Stage2ClosedInputGuardsRemainPresent) {
    const auto path = inputPath();
    std::remove(path->utf8().c_str());
    {
        const auto output = ::jxx::NEW<::jxx::io::FileOutputStream>(path);
        output->write(1);
        output->close();
    }
    const auto input = ::jxx::NEW<::jxx::io::FileInputStream>(path);
    input->close();
    EXPECT_THROW((void)input->read(), ::jxx::io::IOException);
    EXPECT_THROW((void)input->skip(1), ::jxx::io::IOException);
    std::remove(path->utf8().c_str());
}

} // namespace
