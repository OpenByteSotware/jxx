#include "util/zip/jxx.util.zip.Adler32.h"

#include <algorithm>

#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "nio/jxx.nio.ByteBuffer.h"

namespace jxx::util::zip {

Adler32::Adler32() noexcept = default;

void Adler32::update(::jxx::lang::jint value) {
    const auto low = (adler_ & 0xffffU) + static_cast<std::uint8_t>(value);
    const auto high = ((adler_ >> 16U) + low) % MOD_ADLER;
    adler_ = (high << 16U) | (low % MOD_ADLER);
}

void Adler32::update(const ::jxx::lang::ByteArray& buffer) {
    if (buffer == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    update(buffer, 0, buffer->length);
}

void Adler32::update(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {
    if (buffer == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (offset < 0 || length < 0 || offset > buffer->length - length) {
        throw ::jxx::lang::ArrayIndexOutOfBoundsException();
    }

    std::uint32_t low = adler_ & 0xffffU;
    std::uint32_t high = adler_ >> 16U;
    ::jxx::lang::jint remaining = length;
    ::jxx::lang::jint position = offset;

    while (remaining > 0) {
        const auto block = std::min<::jxx::lang::jint>(remaining, 5552);
        remaining -= block;
        for (::jxx::lang::jint index = 0; index < block; ++index) {
            low += static_cast<std::uint8_t>((*buffer)[position++]);
            high += low;
        }
        low %= MOD_ADLER;
        high %= MOD_ADLER;
    }

    adler_ = (high << 16U) | low;
}

void Adler32::update(const ::jxx::Ptr<::jxx::nio::ByteBuffer>& buffer) {
    if (buffer == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    while (buffer->hasRemaining()) {
        update(static_cast<::jxx::lang::jint>(buffer->get()));
    }
}

::jxx::lang::jlong Adler32::getValue() const noexcept {
    return static_cast<::jxx::lang::jlong>(adler_);
}

void Adler32::reset() noexcept {
    adler_ = 1U;
}

} // namespace jxx::util::zip
