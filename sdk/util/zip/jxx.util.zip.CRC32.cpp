#include "util/zip/jxx.util.zip.CRC32.h"

#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "nio/jxx.nio.ByteBuffer.h"

namespace jxx::util::zip {
namespace {

std::array<std::uint32_t, 256U> makeTable() noexcept {
    std::array<std::uint32_t, 256U> result{};

    for (std::uint32_t index = 0U; index < result.size(); ++index) {
        std::uint32_t value = index;
        for (std::uint32_t bit = 0U; bit < 8U; ++bit) {
            value = (value & 1U) != 0U
                ? (value >> 1U) ^ 0xEDB88320U
                : value >> 1U;
        }
        result[index] = value;
    }

    return result;
}

} // namespace

CRC32::CRC32() noexcept = default;

const std::array<std::uint32_t, 256U>& CRC32::table() noexcept {
    static const std::array<std::uint32_t, 256U> value = makeTable();
    return value;
}

void CRC32::updateByte(std::uint8_t value) noexcept {
    const std::uint32_t tableIndex =
        (crc_ ^ static_cast<std::uint32_t>(value)) & 0xFFU;
    crc_ = table()[tableIndex] ^ (crc_ >> 8U);
}

void CRC32::update(::jxx::lang::jint value) {
    updateByte(static_cast<std::uint8_t>(value & 0xFF));
}

void CRC32::update(const ::jxx::lang::ByteArray& buffer) {
    if (buffer == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    update(buffer, 0, buffer->length);
}

void CRC32::update(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {
    if (buffer == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    if (offset < 0 || length < 0 ||
        offset > buffer->length - length) {
        throw ::jxx::lang::ArrayIndexOutOfBoundsException();
    }

    for (::jxx::lang::jint index = 0; index < length; ++index) {
        updateByte(static_cast<std::uint8_t>((*buffer)[offset + index]));
    }
}

void CRC32::update(
    const ::jxx::Ptr<::jxx::nio::ByteBuffer>& buffer) {
    if (buffer == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    while (buffer->hasRemaining()) {
        updateByte(static_cast<std::uint8_t>(buffer->get()));
    }
}

::jxx::lang::jlong CRC32::getValue() const noexcept {
    return static_cast<::jxx::lang::jlong>(crc_ ^ 0xFFFFFFFFU);
}

void CRC32::reset() noexcept {
    crc_ = 0xFFFFFFFFU;
}

} // namespace jxx::util::zip
