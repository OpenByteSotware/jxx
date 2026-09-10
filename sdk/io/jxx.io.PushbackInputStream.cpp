#include "io/jxx.io.PushbackInputStream.h"

#include <algorithm>
#include <cstdint>

#include "io/jxx.io.IOException.h"
#include "io/jxx.io.IOHelper.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::io {

PushbackInputStream::PushbackInputStream(
    const ::jxx::Ptr<InputStream>& input)
    : PushbackInputStream(input, 1) {
}

PushbackInputStream::PushbackInputStream(
    const ::jxx::Ptr<InputStream>& input,
    ::jxx::lang::jint size)
    : Super(input)
    , buffer_(
          ::jxx::NEW<::jxx::lang::ByteArrayType>(
              static_cast<std::uint32_t>(
                  validateBufferSize_(size))))
    , position_(size)
    , closed_(false) {
}

PushbackInputStream::~PushbackInputStream() = default;

::jxx::lang::jint
PushbackInputStream::validateBufferSize_(
    ::jxx::lang::jint size) {

    if (size <= 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }

    return size;
}

void PushbackInputStream::ensureOpen() const {
    if (closed_ || buffer_ == nullptr || in_ == nullptr) {
        throw IOException();
    }
}

::jxx::lang::jint PushbackInputStream::read() {
    ensureOpen();

    if (position_ <
        static_cast<::jxx::lang::jint>(buffer_->length)) {
        return static_cast<::jxx::lang::jint>(
            static_cast<unsigned char>((*buffer_)[position_++]));
    }

    return in_->read();
}

::jxx::lang::jint PushbackInputStream::read(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    ensureOpen();
    IOHelper::checkBounds(buffer, offset, length);

    if (length == 0) {
        return 0;
    }

    const auto buffered =
        static_cast<::jxx::lang::jint>(buffer_->length) -
        position_;

    const auto copied =
        std::min(length, buffered);

    for (::jxx::lang::jint index = 0;
         index < copied;
         ++index) {
        (*buffer)[offset + index] =
            (*buffer_)[position_ + index];
    }

    position_ += copied;

    if (copied == length) {
        return copied;
    }

    const auto readCount =
        in_->read(
            buffer,
            offset + copied,
            length - copied);

    if (readCount < 0) {
        return copied == 0 ? -1 : copied;
    }

    return copied + readCount;
}

void PushbackInputStream::unread(
    ::jxx::lang::jint value) {

    ensureOpen();

    if (position_ == 0) {
        throw IOException();
    }

    (*buffer_)[--position_] =
        static_cast<::jxx::lang::jbyte>(value);
}

void PushbackInputStream::unread(
    const ::jxx::lang::ByteArray& buffer) {

    if (buffer == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    unread(
        buffer,
        0,
        static_cast<::jxx::lang::jint>(buffer->length));
}

void PushbackInputStream::unread(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    ensureOpen();
    IOHelper::checkBounds(buffer, offset, length);

    if (length > position_) {
        throw IOException();
    }

    position_ -= length;

    for (::jxx::lang::jint index = 0;
         index < length;
         ++index) {
        (*buffer_)[position_ + index] =
            (*buffer)[offset + index];
    }
}

::jxx::lang::jint PushbackInputStream::available() {
    ensureOpen();

    return
        static_cast<::jxx::lang::jint>(buffer_->length) -
        position_ +
        in_->available();
}

::jxx::lang::jlong PushbackInputStream::skip(
    ::jxx::lang::jlong count) {

    ensureOpen();

    if (count <= 0) {
        return 0;
    }

    const auto buffered =
        static_cast<::jxx::lang::jlong>(buffer_->length) -
        position_;

    const auto bufferedSkip =
        std::min(count, buffered);

    position_ +=
        static_cast<::jxx::lang::jint>(bufferedSkip);

    if (bufferedSkip == count) {
        return bufferedSkip;
    }

    return bufferedSkip +
        in_->skip(count - bufferedSkip);
}

::jxx::lang::jbool
PushbackInputStream::markSupported() const {
    return false;
}

void PushbackInputStream::mark(
    ::jxx::lang::jint readLimit) {
    (void)readLimit;
}

void PushbackInputStream::reset() {
    throw IOException();
}

void PushbackInputStream::close() {
    if (closed_) {
        return;
    }

    closed_ = true;
    buffer_.reset();

    if (in_ != nullptr) {
        in_->close();
        in_.reset();
    }
}

} // namespace jxx::io
