#include <gtest/gtest.h>

#include "io/jxx.io.File.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "io/jxx.io.FileOutputStream.h"
#include "lang/jxx.lang.String.h"

namespace {
TEST(FileOutputStreamDirectoryTest, DirectoryCannotBeOpenedForOverwriteOrAppend) {
    const auto path=::jxx::NEW<::jxx::lang::String>(".");
    const auto file=::jxx::NEW<::jxx::io::File>(path);
    EXPECT_THROW(::jxx::NEW<::jxx::io::FileOutputStream>(path),
                 ::jxx::io::FileNotFoundException);
    EXPECT_THROW(::jxx::NEW<::jxx::io::FileOutputStream>(path,true),
                 ::jxx::io::FileNotFoundException);
    EXPECT_THROW(::jxx::NEW<::jxx::io::FileOutputStream>(file),
                 ::jxx::io::FileNotFoundException);
    EXPECT_THROW(::jxx::NEW<::jxx::io::FileOutputStream>(file,true),
                 ::jxx::io::FileNotFoundException);
}
}
