#include <gtest/gtest.h>

#include <cstdio>

#include "io/jxx.io.File.h"
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "io/jxx.io.FileOutputStream.h"
#include "lang/jxx.lang.String.h"

namespace {
TEST(FileOutputStreamCreationModesTest, MissingParentThrowsAndDoesNotCreateTarget) {
    const auto path=::jxx::NEW<::jxx::lang::String>(
        "jxx_missing_parent_dir/child.tmp");
    const auto file=::jxx::NEW<::jxx::io::File>(path);
    EXPECT_THROW(::jxx::NEW<::jxx::io::FileOutputStream>(path),
                 ::jxx::io::FileNotFoundException);
    EXPECT_FALSE(file->exists());
}

TEST(FileOutputStreamCreationModesTest, OverwriteTruncatesAndAppendPreserves) {
    const auto path=::jxx::NEW<::jxx::lang::String>("jxx_output_modes.tmp");
    std::remove(path->utf8().c_str());
    {const auto out=::jxx::NEW<::jxx::io::FileOutputStream>(path);out->write(1);out->write(2);out->close();}
    {const auto out=::jxx::NEW<::jxx::io::FileOutputStream>(path);out->write(3);out->close();}
    {const auto out=::jxx::NEW<::jxx::io::FileOutputStream>(path,true);out->write(4);out->close();}
    const auto in=::jxx::NEW<::jxx::io::FileInputStream>(path);
    EXPECT_EQ(3,in->read()); EXPECT_EQ(4,in->read()); EXPECT_EQ(-1,in->read());
    in->close(); std::remove(path->utf8().c_str());
}
}
