#include "security/jxx.security.SecureString.h"

#include <cstring>
#include <limits>
#include <utility>

#include "lang/jxx.lang.Exceptions.h"

namespace jxx::security {

SecureString::SecureString()
    : Super() {
}

SecureString::SecureString(
    const ::jxx::Ptr<::jxx::lang::String>& value)
    : Super() {
    assign(value);
}

SecureString::SecureString(
    const ::jxx::lang::ByteArray& value)
    : Super() {
    assign(value);
}

SecureString::SecureString(
    const ::jxx::lang::ByteArray& value,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length)
    : Super() {
    assign(value, offset, length);
}

SecureString::SecureString(SecureString&& other) noexcept
    : Super() {
    moveFrom_(std::move(other));
}

SecureString& SecureString::operator=(SecureString&& other) noexcept {
    if (this != &other) {
        secureWipe_();
        moveFrom_(std::move(other));
    }
    return *this;
}

SecureString::~SecureString() {
    secureWipe_();
}

void SecureString::assign(
    const ::jxx::Ptr<::jxx::lang::String>& value) {
    if (value == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    const auto utf8 = value->utf8();
    assignNative_(utf8.data(), utf8.size());
}

void SecureString::assign(const ::jxx::lang::ByteArray& value) {
    if (value == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    assign(value, 0, value->length);
}

void SecureString::assign(
    const ::jxx::lang::ByteArray& value,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint lengthValue) {
    if (value == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (offset < 0 || lengthValue < 0 ||
        offset > value->length ||
        lengthValue > value->length - offset) {
        throw ::jxx::lang::IndexOutOfBoundsException();
    }

    secureWipe_();
    buffer_.resize(static_cast<std::size_t>(lengthValue) + 1U);
    for (::jxx::lang::jint index = 0; index < lengthValue; ++index) {
        buffer_[static_cast<std::size_t>(index)] =
            static_cast<char>((*value)[offset + index]);
    }
    buffer_[static_cast<std::size_t>(lengthValue)] = '\0';
}

void SecureString::clear() noexcept {
    secureWipe_();
}

void SecureString::append(::jxx::lang::jbyte value) {
    const auto currentLength = buffer_.empty()
        ? 0U
        : buffer_.size() - 1U;
    if (buffer_.empty()) {
        buffer_.push_back('\0');
    }
    buffer_.insert(
        buffer_.begin() + static_cast<std::ptrdiff_t>(currentLength),
        static_cast<char>(value));
}

::jxx::lang::jint SecureString::length() const noexcept {
    const auto nativeLength = buffer_.empty() ? 0U : buffer_.size() - 1U;
    const auto maximum = static_cast<std::size_t>(
        std::numeric_limits<::jxx::lang::jint>::max());
    return static_cast<::jxx::lang::jint>(
        nativeLength > maximum ? maximum : nativeLength);
}

::jxx::lang::jbool SecureString::isEmpty() const noexcept {
    return length() == 0;
}

::jxx::Ptr<::jxx::lang::String> SecureString::toString() const {
    return ::jxx::NEW<::jxx::lang::String>(
        buffer_.empty() ? std::string() :
        std::string(buffer_.data(), buffer_.size() - 1U));
}

::jxx::lang::ByteArray SecureString::toByteArray() const {
    const auto result = ::jxx::NEW<::jxx::lang::ByteArrayType>(length());
    for (::jxx::lang::jint index = 0; index < result->length; ++index) {
        (*result)[index] = static_cast<::jxx::lang::jbyte>(
            buffer_[static_cast<std::size_t>(index)]);
    }
    return result;
}

void SecureString::assignNative_(const char* data, std::size_t lengthValue) {
    secureWipe_();
    buffer_.resize(lengthValue + 1U);
    if (lengthValue != 0U) {
        std::memcpy(buffer_.data(), data, lengthValue);
    }
    buffer_[lengthValue] = '\0';
}

void SecureString::moveFrom_(SecureString&& other) noexcept {
    buffer_ = std::move(other.buffer_);
    other.secureWipe_();
}

void SecureString::secureWipe_() noexcept {
    if (buffer_.empty()) {
        return;
    }
    volatile char* data =
        reinterpret_cast<volatile char*>(buffer_.data());
    for (std::size_t index = 0; index < buffer_.size(); ++index) {
        data[index] = 0;
    }
    buffer_.clear();
    buffer_.shrink_to_fit();
}

} // namespace jxx::security
