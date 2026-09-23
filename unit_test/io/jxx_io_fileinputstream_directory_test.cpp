#include <gtest/gtest.h>
#include "io/jxx.io.File.h"
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "lang/jxx.lang.String.h"
namespace {
TEST(FileInputStreamDirectoryTest, DirectoryCannotBeOpenedAsByteInput) {
    const auto path=::jxx::NEW<::jxx::lang::String>(".");
    EXPECT_THROW(::jxx::NEW<::jxx::io::FileInputStream>(path),::jxx::io::FileNotFoundException);
    EXPECT_THROW(::jxx::NEW<::jxx::io::FileInputStream>(::jxx::NEW<::jxx::io::File>(path)),::jxx::io::FileNotFoundException);
}
}
