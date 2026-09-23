#include <gtest/gtest.h>
#include <cstdio>
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
#include "lang/jxx.lang.String.h"
namespace {
TEST(FileOutputStreamZeroLengthTest, ZeroLengthRangeAtArrayEndWritesNothing) {
    const auto path=::jxx::NEW<::jxx::lang::String>("jxx_zero_length_output.tmp");
    std::remove(path->utf8().c_str());
    const auto output=::jxx::NEW<::jxx::io::FileOutputStream>(path);
    auto bytes=std::make_shared<::jxx::lang::JxxArray<::jxx::lang::jbyte,1U>>(1);
    (*bytes)[0]=7;
    output->write(bytes,1,0);
    output->write(9);
    output->close();
    const auto input=::jxx::NEW<::jxx::io::FileInputStream>(path);
    EXPECT_EQ(9,input->read()); EXPECT_EQ(-1,input->read());
    input->close(); std::remove(path->utf8().c_str());
}
}
