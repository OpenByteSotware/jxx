#include <gtest/gtest.h>
#include <type_traits>

#include "io/jxx.io.FileDescriptor.h"

namespace {
TEST(FileDescriptorEncapsulationTest, DescriptorRemainsPubliclyUsableWithoutExposingInvalidation) {
    EXPECT_TRUE((std::is_default_constructible_v<::jxx::io::FileDescriptor>));
    const auto descriptor=::jxx::NEW<::jxx::io::FileDescriptor>();
    EXPECT_FALSE(descriptor->valid());
}
}
