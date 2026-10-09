#include <gtest/gtest.h>

#include "nio/jxx.nio.CharBuffer.h"

TEST(JxxCharBufferPortabilityTest, AllocateAndViewOperationsRemainUsable) {
    const auto buffer = ::jxx::nio::CharBuffer::allocate(8);
    ASSERT_NE(buffer, nullptr);
    buffer->put(static_cast<::jxx::lang::jchar>('A'));
    buffer->flip();
    EXPECT_EQ(buffer->get(), static_cast<::jxx::lang::jchar>('A'));

    const auto duplicate = buffer->duplicate();
    const auto readOnly = buffer->asReadOnlyBuffer();
    EXPECT_NE(duplicate, nullptr);
    EXPECT_NE(readOnly, nullptr);
}
