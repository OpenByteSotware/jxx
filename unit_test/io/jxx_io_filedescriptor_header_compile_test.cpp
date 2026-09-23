#include <gtest/gtest.h>
#include <type_traits>
#include "io/jxx.io.FileDescriptor.h"
namespace {
TEST(FileDescriptorHeaderCompileTest, HeaderDoesNotRequireCstdio) {
    EXPECT_TRUE((std::is_destructible_v<::jxx::io::FileDescriptor>));
}
}
