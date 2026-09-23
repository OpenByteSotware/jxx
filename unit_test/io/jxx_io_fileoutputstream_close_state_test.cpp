#include <gtest/gtest.h>

#include <cstdio>

#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.String.h"

namespace {
TEST(FileOutputStreamCloseStateTest, ClosePersistsBytesAndRemainsIdempotent) {
    const auto path=::jxx::NEW<::jxx::lang::String>("jxx_fileoutputstream_close_state.tmp");
    std::remove(path->utf8().c_str());
    const auto output=::jxx::NEW<::jxx::io::FileOutputStream>(path);
    output->write(0x41);
    output->close();
    EXPECT_NO_THROW(output->close());
    EXPECT_THROW(output->write(0x42),::jxx::io::IOException);
    const auto input=::jxx::NEW<::jxx::io::FileInputStream>(path);
    EXPECT_EQ(0x41,input->read());
    EXPECT_EQ(-1,input->read());
    input->close();
    std::remove(path->utf8().c_str());
}
}
