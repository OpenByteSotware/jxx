#include <gtest/gtest.h>
#include "io/jxx.io.FileDescriptor.h"
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace {
TEST(FileInputStreamDescriptorValidationTest, RejectsNullDescriptor) {
    ::jxx::Ptr<::jxx::io::FileDescriptor> descriptor;
    EXPECT_THROW(
        ::jxx::NEW<::jxx::io::FileInputStream>(descriptor),
        ::jxx::lang::NullPointerException);
}
TEST(FileInputStreamDescriptorValidationTest, RejectsInvalidDescriptor) {
    const auto descriptor=::jxx::NEW<::jxx::io::FileDescriptor>();
    EXPECT_THROW(
        ::jxx::NEW<::jxx::io::FileInputStream>(descriptor),
        ::jxx::io::FileNotFoundException);
}
}
