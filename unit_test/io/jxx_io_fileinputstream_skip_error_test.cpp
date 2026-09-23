#include <gtest/gtest.h>
#include <cstdio>
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
#include "lang/jxx.lang.String.h"
namespace {
TEST(FileInputStreamSkipTest, PositiveAndNonPositiveSkipPreserveExpectedPosition) {
    const auto path=::jxx::NEW<::jxx::lang::String>("jxx_fileinputstream_skip_test.tmp");
    std::remove(path->utf8().c_str());
    {const auto output=::jxx::NEW<::jxx::io::FileOutputStream>(path);for(int value=1;value<=5;++value)output->write(value);output->close();}
    const auto input=::jxx::NEW<::jxx::io::FileInputStream>(path);
    EXPECT_EQ(0,input->skip(0));
    EXPECT_EQ(0,input->skip(-5));
    EXPECT_EQ(2,input->skip(2));
    EXPECT_EQ(3,input->read());
    input->close();
    std::remove(path->utf8().c_str());
}
}
