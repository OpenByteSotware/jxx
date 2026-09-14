#include "io/jxx.io.ByteArrayInputStream.h"

#include <algorithm>

#include "io/jxx.io.IOHelper.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::io {

ByteArrayInputStream::ByteArrayInputStream(
    const ::jxx::lang::ByteArray& buffer)
    : ByteArrayInputStream(
          buffer,
          0,
          buffer == nullptr
              ? 0
              : static_cast<::jxx::lang::jint>(buffer->length)) {
}

ByteArrayInputStream::ByteArrayInputStream(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length)
    : Super()
    , buffer_(buffer) {

    if (buffer == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    const auto size =
        static_cast<::jxx::lang::jint>(buffer->length);
    position_ = std::min(offset, size);
    mark_ = position_;
    count_ = std::min(offset + length, size);
}

::jxx::lang::jint ByteArrayInputStream::read() {
    return synchronized([&]() -> ::jxx::lang::jint {
        return position_ < count_
            ? static_cast<::jxx::lang::jint>(
                  static_cast<unsigned char>((*buffer_)[position_++]))
            : -1;
    });
}

::jxx::lang::jint ByteArrayInputStream::read(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    return synchronized([&]() -> ::jxx::lang::jint {
        IOHelper::checkBounds(buffer, offset, length);
        if (length == 0) {
            return 0;
        }
        if (position_ >= count_) {
            return -1;
        }

        const auto amount = std::min(length, count_ - position_);
        for (::jxx::lang::jint index = 0; index < amount; ++index) {
            (*buffer)[offset + index] = (*buffer_)[position_ + index];
        }
        position_ += amount;
        return amount;
    });
}

::jxx::lang::jlong ByteArrayInputStream::skip(
    ::jxx::lang::jlong count) {

    return synchronized([&]() -> ::jxx::lang::jlong {
        const auto amount = std::min<::jxx::lang::jlong>(
            std::max<::jxx::lang::jlong>(count, 0),
            count_ - position_);
        position_ += static_cast<::jxx::lang::jint>(amount);
        return amount;
    });
}

::jxx::lang::jint ByteArrayInputStream::available() {
    return synchronized([&]() -> ::jxx::lang::jint {
        return count_ - position_;
    });
}

void ByteArrayInputStream::close() {
}

void ByteArrayInputStream::mark(::jxx::lang::jint readAheadLimit) {
    (void)readAheadLimit;
    synchronized([&] {
        mark_ = position_;
    });
}

void ByteArrayInputStream::reset() {
    synchronized([&] {
        position_ = mark_;
    });
}

::jxx::lang::jbool ByteArrayInputStream::markSupported() const {
    return true;
}

} // namespace jxx::io
