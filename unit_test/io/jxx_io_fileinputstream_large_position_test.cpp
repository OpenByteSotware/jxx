#include <gtest/gtest.h>
#include <cstdio>
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
#include "lang/jxx.lang.String.h"
namespace {
TEST(FileInputStreamLargePositionTest, SkipAndAvailableUseWideNativePositions) {
    const auto path=::jxx::NEW<::jxx::lang::String>("jxx_large_position_test.tmp");
    std::remove(path->utf8().c_str());
    {const auto output=::jxx::NEW<::jxx::io::FileOutputStream>(path);for(int i=0;i<16;++i)output->write(i);output->close();}
    const auto input=::jxx::NEW<::jxx::io::FileInputStream>(path);
    EXPECT_EQ(12,input->skip(12)); EXPECT_EQ(4,input->available()); EXPECT_EQ(12,input->read());
    input->close(); std::remove(path->utf8().c_str());
}
}
