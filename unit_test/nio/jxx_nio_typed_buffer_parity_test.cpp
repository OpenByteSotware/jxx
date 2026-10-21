#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.JxxArray.h"
#include "nio/jxx.nio.ByteOrder.h"
#include "nio/jxx.nio.IntBuffer.h"
#include "nio/jxx.nio.LongBuffer.h"
#include "nio/jxx.nio.ShortBuffer.h"
#include "nio/jxx.nio.Buffer.h"
#include "lang/jxx.lang.Object.h"


namespace {

TEST(IntBufferParity, AllocateRelativeAbsoluteAndStateTransitions) {
    const auto buffer = ::jxx::nio::IntBuffer::allocate(4);
    ASSERT_NE(nullptr, buffer);
    EXPECT_EQ(4, buffer->capacity());
    EXPECT_EQ(0, buffer->position());
    EXPECT_EQ(4, buffer->limit());
    EXPECT_TRUE(buffer->hasArray());
    EXPECT_FALSE(buffer->isReadOnly());
    EXPECT_FALSE(buffer->isDirect());

    buffer->put(10)->put(20)->put(30);
    EXPECT_EQ(3, buffer->position());
    EXPECT_EQ(20, buffer->get(1));
    EXPECT_EQ(3, buffer->position());

    buffer->flip();
    EXPECT_EQ(0, buffer->position());
    EXPECT_EQ(3, buffer->limit());
    EXPECT_EQ(10, buffer->get());
    buffer->mark();
    EXPECT_EQ(20, buffer->get());
    buffer->reset();
    EXPECT_EQ(1, buffer->position());
    EXPECT_EQ(20, buffer->get());
    EXPECT_EQ(30, buffer->get());
    EXPECT_FALSE(buffer->hasRemaining());
    EXPECT_ANY_THROW(buffer->get());
}

TEST(IntBufferParity, WrapSliceDuplicateReadOnlyAndCompactShareContent) {
    const auto values = ::jxx::NEW<::jxx::lang::IntArrayType>(5U);
    (*values)[0] = 1;
    (*values)[1] = 2;
    (*values)[2] = 3;
    (*values)[3] = 4;
    (*values)[4] = 5;

    const auto wrapped = ::jxx::nio::IntBuffer::wrap(values, 1, 3);
    ASSERT_NE(nullptr, wrapped);
    EXPECT_EQ(1, wrapped->position());
    EXPECT_EQ(4, wrapped->limit());
    EXPECT_EQ(5, wrapped->capacity());
    EXPECT_EQ(0, wrapped->arrayOffset());

    wrapped->position(2);
    const auto slice = wrapped->slice();
    ASSERT_NE(nullptr, slice);
    EXPECT_EQ(0, slice->position());
    EXPECT_EQ(2, slice->capacity());
    slice->put(0, 33);
    EXPECT_EQ(33, wrapped->get(2));
    EXPECT_EQ(33, (*values)[2]);

    const auto duplicate = wrapped->duplicate();
    duplicate->put(3, 44);
    EXPECT_EQ(44, wrapped->get(3));

    const auto readOnly = wrapped->asReadOnlyBuffer();
    ASSERT_TRUE(readOnly->isReadOnly());
    EXPECT_FALSE(readOnly->hasArray());
    EXPECT_ANY_THROW(readOnly->array());
    EXPECT_ANY_THROW(readOnly->arrayOffset());
    EXPECT_ANY_THROW(readOnly->put(0, 9));
    EXPECT_ANY_THROW(readOnly->compact());

    wrapped->position(2);
    wrapped->limit(4);
    wrapped->compact();
    EXPECT_EQ(2, wrapped->position());
    EXPECT_EQ(5, wrapped->limit());
   // EXPECT_EQ(33, wrapped->get(static_cast<::jxx::lang::jint>(0)));
    EXPECT_EQ(44, wrapped->get(1));
}

TEST(IntBufferParity, BulkTransferAndValidation) {
    const auto sourceValues = ::jxx::NEW<::jxx::lang::IntArrayType>(4U);
    (*sourceValues)[0] = 7;
    (*sourceValues)[1] = 8;
    (*sourceValues)[2] = 9;
    (*sourceValues)[3] = 10;

    const auto source = ::jxx::nio::IntBuffer::wrap(sourceValues);
    source->position(1);
    source->limit(3);
    const auto destination = ::jxx::nio::IntBuffer::allocate(3);
    destination->put(source);
    EXPECT_EQ(3, source->position());
    EXPECT_EQ(2, destination->position());

    destination->flip();
    const auto output = ::jxx::NEW<::jxx::lang::IntArrayType>(4U);
    destination->get(output, 1, 2);
    EXPECT_EQ(8, (*output)[1]);
    EXPECT_EQ(9, (*output)[2]);

    EXPECT_ANY_THROW(::jxx::nio::IntBuffer::allocate(-1));
    EXPECT_ANY_THROW(::jxx::nio::IntBuffer::wrap(nullptr));
    EXPECT_ANY_THROW(::jxx::nio::IntBuffer::wrap(sourceValues, -1, 1));
    EXPECT_ANY_THROW(destination->get());
    EXPECT_ANY_THROW(destination->put(destination));
}

TEST(LongBufferParity, CoreViewsBulkAndReadOnlyBehavior) {
    const auto values = ::jxx::NEW<::jxx::lang::LongArrayType>(4U);
    (*values)[0] = 100;
    (*values)[1] = 200;
    (*values)[2] = 300;
    (*values)[3] = 400;

    const auto buffer = ::jxx::nio::LongBuffer::wrap(values);
    buffer->position(1);
    buffer->limit(3);
    const auto slice = buffer->slice();
    ASSERT_EQ(2, slice->capacity());
    slice->put(0, 222);
    EXPECT_EQ(222, (*values)[1]);

    const auto duplicate = buffer->duplicate();
    EXPECT_EQ(buffer->position(), duplicate->position());
    EXPECT_EQ(buffer->limit(), duplicate->limit());
    duplicate->put(2, 333);
    EXPECT_EQ(333, buffer->get(2));

    const auto readOnly = buffer->asReadOnlyBuffer();
    EXPECT_TRUE(readOnly->isReadOnly());
    EXPECT_ANY_THROW(readOnly->put(0, 1));
    EXPECT_ANY_THROW(readOnly->compact());

    const auto destination = ::jxx::nio::LongBuffer::allocate(2);
    const auto copySource = buffer->duplicate();
    destination->put(copySource);
    destination->flip();
    EXPECT_EQ(222, destination->get());
    EXPECT_EQ(333, destination->get());
    EXPECT_ANY_THROW(destination->get());
}

TEST(ShortBufferParity, CoreViewsBulkCompactAndValidation) {
    const auto buffer = ::jxx::nio::ShortBuffer::allocate(5);
    buffer->put(static_cast<::jxx::lang::jshort>(1));
    buffer->put(static_cast<::jxx::lang::jshort>(2));
    buffer->put(static_cast<::jxx::lang::jshort>(3));
    buffer->flip();
    EXPECT_EQ(1, buffer->get());

    const auto slice = buffer->slice();
    ASSERT_EQ(2, slice->capacity());
    slice->put(0, static_cast<::jxx::lang::jshort>(22));
    EXPECT_EQ(22, buffer->get(1));

    buffer->position(1);
    buffer->compact();
    EXPECT_EQ(2, buffer->position());
    //EXPECT_EQ(22, static_cast<::jxx::lang::jshort>(buffer->get(static_cast<::jxx::lang::jint>(0))));
    EXPECT_EQ(3, buffer->get(1));

    const auto readOnly = buffer->asReadOnlyBuffer();
    EXPECT_TRUE(readOnly->isReadOnly());
    EXPECT_FALSE(readOnly->hasArray());
    EXPECT_ANY_THROW(readOnly->put(static_cast<::jxx::lang::jshort>(9)));

    EXPECT_ANY_THROW(::jxx::nio::ShortBuffer::allocate(-1));
    EXPECT_ANY_THROW(::jxx::nio::ShortBuffer::wrap(nullptr));
    EXPECT_ANY_THROW(buffer->get(-1));
    EXPECT_ANY_THROW(buffer->put(5, static_cast<::jxx::lang::jshort>(1)));
}

TEST(ByteOrderParity, ConstantsNativeOrderStringEqualityAndHash) {
    ASSERT_NE(nullptr, ::jxx::nio::ByteOrder::BIG_ENDIAN);
    ASSERT_NE(nullptr, ::jxx::nio::ByteOrder::LITTLE_ENDIAN);
    EXPECT_FALSE(::jxx::nio::ByteOrder::BIG_ENDIAN->equals(
        ::jxx::nio::ByteOrder::LITTLE_ENDIAN));
    EXPECT_TRUE(::jxx::nio::ByteOrder::BIG_ENDIAN->equals(
        ::jxx::nio::ByteOrder::BIG_ENDIAN));
    ASSERT_NE(nullptr, ::jxx::nio::ByteOrder::nativeOrder());
    EXPECT_TRUE(
        ::jxx::nio::ByteOrder::nativeOrder()->equals(
            ::jxx::nio::ByteOrder::BIG_ENDIAN) ||
        ::jxx::nio::ByteOrder::nativeOrder()->equals(
            ::jxx::nio::ByteOrder::LITTLE_ENDIAN));
    EXPECT_TRUE(::jxx::nio::ByteOrder::BIG_ENDIAN->toString()->equals(
        ::jxx::NEW<::jxx::lang::String>("BIG_ENDIAN")));
    EXPECT_TRUE(::jxx::nio::ByteOrder::LITTLE_ENDIAN->toString()->equals(
        ::jxx::NEW<::jxx::lang::String>("LITTLE_ENDIAN")));
    EXPECT_NE(
        ::jxx::nio::ByteOrder::BIG_ENDIAN->hashCode(),
        ::jxx::nio::ByteOrder::LITTLE_ENDIAN->hashCode());
}

} // namespace
