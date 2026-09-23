#include <gtest/gtest.h>

#include <cstdio>

#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.String.h"

namespace {

::jxx::Ptr<::jxx::lang::String> postClosePath() {
    return ::jxx::NEW<::jxx::lang::String>(
        "jxx_file_stream_post_close_test.tmp");
}

TEST(FileStreamPostCloseTest, OutputOperationsThrowIOExceptionAfterClose) {
    const auto path = postClosePath();
    std::remove(path->utf8().c_str());
    const auto output =
        ::jxx::NEW<::jxx::io::FileOutputStream>(path);
    auto bytes = std::make_shared<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(1);
    output->close();

    EXPECT_THROW(output->write(1), ::jxx::io::IOException);
    EXPECT_THROW(output->write(bytes, 0, 1), ::jxx::io::IOException);
    EXPECT_THROW(output->flush(), ::jxx::io::IOException);
    EXPECT_THROW((void)output->getFD(), ::jxx::io::IOException);
    EXPECT_NO_THROW(output->close());

    std::remove(path->utf8().c_str());
}

TEST(FileStreamPostCloseTest, InputOperationsThrowIOExceptionAfterClose) {
    const auto path = postClosePath();
    std::remove(path->utf8().c_str());
    {
        const auto output =
            ::jxx::NEW<::jxx::io::FileOutputStream>(path);
        output->write(1);
        output->close();
    }

    const auto input =
        ::jxx::NEW<::jxx::io::FileInputStream>(path);
    auto bytes = std::make_shared<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(1);
    input->close();

    EXPECT_THROW((void)input->read(), ::jxx::io::IOException);
    EXPECT_THROW(input->read(bytes, 0, 1), ::jxx::io::IOException);
    EXPECT_THROW((void)input->skip(1), ::jxx::io::IOException);
    EXPECT_THROW((void)input->available(), ::jxx::io::IOException);
    EXPECT_THROW((void)input->getFD(), ::jxx::io::IOException);
    EXPECT_NO_THROW(input->close());

    std::remove(path->utf8().c_str());
}

} // namespace
