#include "io/jxx.io.ByteArrayOutputStream.h"

#include <algorithm>
#include <cstdint>

#include "io/jxx.io.IOHelper.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

ByteArrayOutputStream::ByteArrayOutputStream()
    : ByteArrayOutputStream(32) {
}

ByteArrayOutputStream::ByteArrayOutputStream(::jxx::lang::jint size)
    : Super() {

    if (size < 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    buffer_ = ::jxx::NEW<::jxx::lang::ByteArrayType>(
        static_cast<std::uint32_t>(size));
}

void ByteArrayOutputStream::ensureCapacity(::jxx::lang::jint capacity) {
    if (capacity <= static_cast<::jxx::lang::jint>(buffer_->length)) {
        return;
    }

    const auto newCapacity = std::max(
        capacity,
        static_cast<::jxx::lang::jint>(buffer_->length) * 2);
    const auto replacement = ::jxx::NEW<::jxx::lang::ByteArrayType>(
        static_cast<std::uint32_t>(newCapacity));
    for (::jxx::lang::jint index = 0; index < count_; ++index) {
        (*replacement)[index] = (*buffer_)[index];
    }
    buffer_ = replacement;
}

void ByteArrayOutputStream::write(::jxx::lang::jint value) {
    synchronized([&] {
        ensureCapacity(count_ + 1);
        (*buffer_)[count_++] = static_cast<::jxx::lang::jbyte>(value);
    });
}

void ByteArrayOutputStream::write(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    synchronized([&] {
        IOHelper::checkBounds(buffer, offset, length);
        ensureCapacity(count_ + length);
        for (::jxx::lang::jint index = 0; index < length; ++index) {
            (*buffer_)[count_ + index] = (*buffer)[offset + index];
        }
        count_ += length;
    });
}

void ByteArrayOutputStream::writeTo(
    const ::jxx::Ptr<OutputStream>& output) const {

    synchronized([&] {
        if (output == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        output->write(buffer_, 0, count_);
    });
}

void ByteArrayOutputStream::reset() {
    synchronized([&] {
        count_ = 0;
    });
}

::jxx::lang::ByteArray ByteArrayOutputStream::toByteArray() const {
    return synchronized([&]() -> ::jxx::lang::ByteArray {
        const auto result = ::jxx::NEW<::jxx::lang::ByteArrayType>(
            static_cast<std::uint32_t>(count_));
        for (::jxx::lang::jint index = 0; index < count_; ++index) {
            (*result)[index] = (*buffer_)[index];
        }
        return result;
    });
}

::jxx::lang::jint ByteArrayOutputStream::size() const {
    return synchronized([&]() -> ::jxx::lang::jint {
        return count_;
    });
}

::jxx::Ptr<::jxx::lang::String>
ByteArrayOutputStream::toString() const {
    return synchronized([&]() -> ::jxx::Ptr<::jxx::lang::String> {
        return ::jxx::NEW<::jxx::lang::String>(toByteArray());
    });
}

void ByteArrayOutputStream::close() {
}

} // namespace jxx::io
