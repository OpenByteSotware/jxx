#include <gtest/gtest.h>
#include <cstdio>
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
#include "lang/jxx.lang.String.h"
namespace {
TEST(FileOutputStreamUnicodePathTest, OverwriteAndAppendUseUnicodePath) {
    const auto path=::jxx::NEW<::jxx::lang::String>(std::u16string(u"jxx_unicode_\u00E9_output.tmp"));
    std::remove(path->utf8().c_str());
    {const auto output=::jxx::NEW<::jxx::io::FileOutputStream>(path);output->write(1);output->close();}
    {const auto output=::jxx::NEW<::jxx::io::FileOutputStream>(path,true);output->write(2);output->close();}
    const auto input=::jxx::NEW<::jxx::io::FileInputStream>(path);
    EXPECT_EQ(1,input->read()); EXPECT_EQ(2,input->read()); EXPECT_EQ(-1,input->read());
    input->close(); std::remove(path->utf8().c_str());
}
}
