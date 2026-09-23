#include <gtest/gtest.h>
#include <cstdio>
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
#include "lang/jxx.lang.String.h"
namespace {
TEST(FileInputStreamAvailableTest, ReportsRemainingBytesWithoutChangingPosition) {
    const auto path=::jxx::NEW<::jxx::lang::String>("jxx_fileinputstream_available_test.tmp");
    std::remove(path->utf8().c_str());
    {const auto output=::jxx::NEW<::jxx::io::FileOutputStream>(path);output->write(10);output->write(20);output->write(30);output->close();}
    const auto input=::jxx::NEW<::jxx::io::FileInputStream>(path);
    EXPECT_EQ(3,input->available());
    EXPECT_EQ(10,input->read());
    EXPECT_EQ(2,input->available());
    EXPECT_EQ(20,input->read());
    EXPECT_EQ(1,input->available());
    input->close();
    std::remove(path->utf8().c_str());
}
}
