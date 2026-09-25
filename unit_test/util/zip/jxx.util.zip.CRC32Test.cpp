#include <gtest/gtest.h>

#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.buildin_array.h"
#include "nio/jxx.nio.ByteBuffer.h"
#include "util/zip/jxx.util.zip.CRC32.h"

namespace {

::jxx::lang::ByteArray bytes(const char* text) {
    ::jxx::lang::jint length = 0;
    while (text[length] != '\0') {
        ++length;
    }

    const auto result =
        ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    for (::jxx::lang::jint index = 0; index < length; ++index) {
        (*result)[index] = static_cast<::jxx::lang::jbyte>(text[index]);
    }
    return result;
}

TEST(CRC32Test, EmptyValueIsZero) {
    const auto crc = ::jxx::NEW<::jxx::util::zip::CRC32>();
    EXPECT_EQ(0, crc->getValue());
}

TEST(CRC32Test, StandardCheckValue) {
    const auto crc = ::jxx::NEW<::jxx::util::zip::CRC32>();
    crc->update(bytes("123456789"));
    EXPECT_EQ(0xCBF43926LL, crc->getValue());
}

TEST(CRC32Test, OffsetLengthAndReset) {
    const auto value = bytes("xx123456789yy");
    const auto crc = ::jxx::NEW<::jxx::util::zip::CRC32>();

    crc->update(value, 2, 9);
    EXPECT_EQ(0xCBF43926LL, crc->getValue());

    crc->reset();
    EXPECT_EQ(0, crc->getValue());
}

TEST(CRC32Test, IntUsesLowEightBits) {
    const auto left = ::jxx::NEW<::jxx::util::zip::CRC32>();
    const auto right = ::jxx::NEW<::jxx::util::zip::CRC32>();

    left->update(0x1234);
    right->update(0x34);

    EXPECT_EQ(right->getValue(), left->getValue());
}

TEST(CRC32Test, ByteBufferConsumesRemainingBytes) {
    const auto source = bytes("xx123456789yy");
    const auto buffer = ::jxx::nio::ByteBuffer::wrap(source);
    buffer->position(2);
    buffer->limit(11);

    const auto crc = ::jxx::NEW<::jxx::util::zip::CRC32>();
    crc->update(buffer);

    EXPECT_EQ(0xCBF43926LL, crc->getValue());
    EXPECT_EQ(buffer->limit(), buffer->position());
}

TEST(CRC32Test, NullAndBoundsValidation) {
    const auto crc = ::jxx::NEW<::jxx::util::zip::CRC32>();
    const auto value = bytes("abc");

    EXPECT_THROW(
        crc->update(::jxx::lang::ByteArray{}),
        ::jxx::lang::NullPointerException);
    EXPECT_THROW(
        crc->update(value, -1, 1),
        ::jxx::lang::ArrayIndexOutOfBoundsException);
    EXPECT_THROW(
        crc->update(value, 1, 3),
        ::jxx::lang::ArrayIndexOutOfBoundsException);
}

} // namespace
