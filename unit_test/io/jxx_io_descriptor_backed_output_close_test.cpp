#include <gtest/gtest.h>
#include <cstdio>
#include "io/jxx.io.FileDescriptor.h"
#include "io/jxx.io.FileOutputStream.h"
#include "io/jxx.io.IOException.h"
namespace {
TEST(DescriptorBackedOutputCloseTest, CloseInvalidatesAndClosesDescriptorConnection) {
    const char* path="jxx_descriptor_output_close.tmp";
    std::remove(path);
    FILE* handle=std::fopen(path,"wb");
    ASSERT_NE(nullptr,handle);
    const auto descriptor=::jxx::NEW<::jxx::io::FileDescriptor>(handle,false);
    const auto output=::jxx::NEW<::jxx::io::FileOutputStream>(descriptor);
    output->write(0x41);
    output->close();
    EXPECT_FALSE(descriptor->valid());
    EXPECT_THROW(output->write(0x42),::jxx::io::IOException);
    std::remove(path);
}
}
