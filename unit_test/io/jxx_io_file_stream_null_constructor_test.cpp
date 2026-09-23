#include <gtest/gtest.h>

#include "io/jxx.io.File.h"
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace {
TEST(FileStreamNullConstructorTest, RejectsNullStringArguments) {
    ::jxx::Ptr<::jxx::lang::String> name;
    EXPECT_THROW(::jxx::NEW<::jxx::io::FileInputStream>(name),
                 ::jxx::lang::NullPointerException);
    EXPECT_THROW(::jxx::NEW<::jxx::io::FileOutputStream>(name),
                 ::jxx::lang::NullPointerException);
    EXPECT_THROW(::jxx::NEW<::jxx::io::FileOutputStream>(name,true),
                 ::jxx::lang::NullPointerException);
}
TEST(FileStreamNullConstructorTest, RejectsNullFileArguments) {
    ::jxx::Ptr<::jxx::io::File> file;
    EXPECT_THROW(::jxx::NEW<::jxx::io::FileInputStream>(file),
                 ::jxx::lang::NullPointerException);
    EXPECT_THROW(::jxx::NEW<::jxx::io::FileOutputStream>(file),
                 ::jxx::lang::NullPointerException);
    EXPECT_THROW(::jxx::NEW<::jxx::io::FileOutputStream>(file,true),
                 ::jxx::lang::NullPointerException);
}
}
