#include <gtest/gtest.h>
#include "io/jxx.io.FileDescriptor.h"
namespace {
TEST(StandardFileDescriptorTest, StandardDescriptorsRemainValid) {
    EXPECT_TRUE(::jxx::io::FileDescriptor::in->valid());
    EXPECT_TRUE(::jxx::io::FileDescriptor::out->valid());
    EXPECT_TRUE(::jxx::io::FileDescriptor::err->valid());
}
}
