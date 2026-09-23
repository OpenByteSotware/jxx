#include <gtest/gtest.h>

#include <cstdio>

#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.String.h"

namespace {
TEST(FileInputStreamCloseStateTest, CloseRemainsIdempotentAndLeavesStreamClosed) {
    const auto path=::jxx::NEW<::jxx::lang::String>("jxx_fileinputstream_close_state.tmp");
    std::remove(path->utf8().c_str());
    {const auto output=::jxx::NEW<::jxx::io::FileOutputStream>(path);output->write(1);output->close();}
    const auto input=::jxx::NEW<::jxx::io::FileInputStream>(path);
    input->close();
    EXPECT_NO_THROW(input->close());
    EXPECT_THROW((void)input->read(),::jxx::io::IOException);
    std::remove(path->utf8().c_str());
}
}
