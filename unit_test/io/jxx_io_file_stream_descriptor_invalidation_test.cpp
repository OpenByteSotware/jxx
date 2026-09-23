#include <gtest/gtest.h>

#include <cstdio>

#include "io/jxx.io.FileDescriptor.h"
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
#include "lang/jxx.lang.String.h"

namespace {
TEST(FileStreamDescriptorInvalidationTest, OutputCloseInvalidatesAssociatedDescriptor) {
    const auto path=::jxx::NEW<::jxx::lang::String>("jxx_output_descriptor_invalidation.tmp");
    std::remove(path->utf8().c_str());
    const auto output=::jxx::NEW<::jxx::io::FileOutputStream>(path);
    const auto descriptor=output->getFD();
    EXPECT_TRUE(descriptor->valid());
    output->close();
    EXPECT_FALSE(descriptor->valid());
    std::remove(path->utf8().c_str());
}
TEST(FileStreamDescriptorInvalidationTest, InputCloseInvalidatesAssociatedDescriptor) {
    const auto path=::jxx::NEW<::jxx::lang::String>("jxx_input_descriptor_invalidation.tmp");
    std::remove(path->utf8().c_str());
    {const auto output=::jxx::NEW<::jxx::io::FileOutputStream>(path);output->write(1);output->close();}
    const auto input=::jxx::NEW<::jxx::io::FileInputStream>(path);
    const auto descriptor=input->getFD();
    EXPECT_TRUE(descriptor->valid());
    input->close();
    EXPECT_FALSE(descriptor->valid());
    std::remove(path->utf8().c_str());
}
}
