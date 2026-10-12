#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "nio/jxx.nio.ClassInitialization.h"
#include "nio/jxx.nio.Buffer.h"
#include "nio/jxx.nio.BufferOverflowException.h"
#include "nio/jxx.nio.BufferUnderflowException.h"
#include "nio/jxx.nio.ByteBuffer.h"
#include "nio/jxx.nio.ByteOrder.h"
#include "nio/jxx.nio.CharBuffer.h"
#include "nio/jxx.nio.DoubleBuffer.h"
#include "nio/jxx.nio.FloatBuffer.h"
#include "nio/jxx.nio.IntBuffer.h"
#include "nio/jxx.nio.InvalidMarkException.h"
#include "nio/jxx.nio.LongBuffer.h"
#include "nio/jxx.nio.MappedByteBuffer.h"
#include "nio/jxx.nio.ShortBuffer.h"

TEST(JxxNioClassInitialization, PublicTypesHaveClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(::jxx::nio::Buffer::Class(), nullptr);
    EXPECT_NE(::jxx::nio::BufferOverflowException::Class(), nullptr);
    EXPECT_NE(::jxx::nio::BufferUnderflowException::Class(), nullptr);
    EXPECT_NE(::jxx::nio::ByteBuffer::Class(), nullptr);
    EXPECT_NE(::jxx::nio::ByteOrder::Class(), nullptr);
    EXPECT_NE(::jxx::nio::CharBuffer::Class(), nullptr);
    EXPECT_NE(::jxx::nio::DoubleBuffer::Class(), nullptr);
    EXPECT_NE(::jxx::nio::FloatBuffer::Class(), nullptr);
    EXPECT_NE(::jxx::nio::IntBuffer::Class(), nullptr);
    EXPECT_NE(::jxx::nio::InvalidMarkException::Class(), nullptr);
    EXPECT_NE(::jxx::nio::LongBuffer::Class(), nullptr);
    EXPECT_NE(::jxx::nio::MappedByteBuffer::Class(), nullptr);
    EXPECT_NE(::jxx::nio::ShortBuffer::Class(), nullptr);
}

TEST(JxxNioClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto bufferBefore = ::jxx::nio::Buffer::Class();
    const auto byteBufferBefore = ::jxx::nio::ByteBuffer::Class();

    EXPECT_NO_THROW(::jxx::nio::initializeClasses());
    EXPECT_NO_THROW(::jxx::nio::initializeClasses());

    EXPECT_EQ(bufferBefore, ::jxx::nio::Buffer::Class());
    EXPECT_EQ(byteBufferBefore, ::jxx::nio::ByteBuffer::Class());
}
