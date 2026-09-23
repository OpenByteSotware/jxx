#include <gtest/gtest.h>

#include "io/jxx.io.FileDescriptor.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "io/jxx.io.FileOutputStream.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace {
TEST(FileOutputStreamDescriptorValidationTest, RejectsNullDescriptor) {
    ::jxx::Ptr<::jxx::io::FileDescriptor> descriptor;
    EXPECT_THROW(::jxx::NEW<::jxx::io::FileOutputStream>(descriptor),
                 ::jxx::lang::NullPointerException);
}
TEST(FileOutputStreamDescriptorValidationTest, RejectsInvalidDescriptor) {
    const auto descriptor=::jxx::NEW<::jxx::io::FileDescriptor>();
    EXPECT_THROW(::jxx::NEW<::jxx::io::FileOutputStream>(descriptor),
                 ::jxx::io::FileNotFoundException);
}
}
