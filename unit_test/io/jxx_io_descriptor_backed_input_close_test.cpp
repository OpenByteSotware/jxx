#include <gtest/gtest.h>
#include <cstdio>
#include "io/jxx.io.FileDescriptor.h"
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.IOException.h"
namespace {
TEST(DescriptorBackedInputCloseTest, CloseInvalidatesAndClosesDescriptorConnection) {
    const char* path="jxx_descriptor_input_close.tmp";
    std::remove(path);
    {FILE* seed=std::fopen(path,"wb");ASSERT_NE(nullptr,seed);std::fputc(0x51,seed);std::fclose(seed);}
    FILE* handle=std::fopen(path,"rb");
    ASSERT_NE(nullptr,handle);
    const auto descriptor=::jxx::NEW<::jxx::io::FileDescriptor>(handle,false);
    const auto input=::jxx::NEW<::jxx::io::FileInputStream>(descriptor);
    EXPECT_EQ(0x51,input->read());
    input->close();
    EXPECT_FALSE(descriptor->valid());
    EXPECT_THROW((void)input->read(),::jxx::io::IOException);
    std::remove(path);
}
}
