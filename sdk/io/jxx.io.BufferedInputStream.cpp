#include "io/jxx.io.BufferedInputStream.h"

#include <algorithm>
#include <cstdint>

#include "io/jxx.io.IOException.h"
#include "io/jxx.io.IOHelper.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::io {

BufferedInputStream::BufferedInputStream(
    const ::jxx::Ptr<InputStream>& input)
    : BufferedInputStream(input, 8192) {
}

BufferedInputStream::BufferedInputStream(
    const ::jxx::Ptr<InputStream>& input,
    ::jxx::lang::jint size)
    : Super(input)
    , buffer_(
          ::jxx::NEW<::jxx::lang::ByteArrayType>(
              static_cast<std::uint32_t>(
                  validateBufferSize_(size)))) {
}

BufferedInputStream::~BufferedInputStream() = default;

::jxx::lang::jint
BufferedInputStream::validateBufferSize_(
    ::jxx::lang::jint size) {

    if (size <= 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }

    return size;
}

void BufferedInputStream::ensureOpen_() const {
    if (closed_ || buffer_ == nullptr || in_ == nullptr) {
        throw IOException();
    }
}

::jxx::lang::jint BufferedInputStream::fill_() {
    ensureOpen_();

    if (markPosition_ < 0) {
        position_ = 0;
    }
    else if (position_ >= static_cast<::jxx::lang::jint>(buffer_->length)) {
        if (markPosition_ > 0) {
            const auto preserved = position_ - markPosition_;

            for (::jxx::lang::jint index = 0;
                 index < preserved;
                 ++index) {
                (*buffer_)[index] =
                    (*buffer_)[markPosition_ + index];
            }

            position_ = preserved;
            count_ = preserved;
            markPosition_ = 0;
        }
        else if (position_ >= markLimit_) {
            markPosition_ = -1;
            position_ = 0;
            count_ = 0;
        }
    }

    const auto capacity =
        static_cast<::jxx::lang::jint>(buffer_->length);

    const auto readCount =
        in_->read(
            buffer_,
            position_,
            capacity - position_);

    if (readCount > 0) {
        count_ = position_ + readCount;
    }

    return readCount;
}

::jxx::lang::jint BufferedInputStream::read() {
    ensureOpen_();

    if (position_ >= count_ && fill_() < 0) {
        return -1;
    }

    return static_cast<::jxx::lang::jint>(
        static_cast<unsigned char>((*buffer_)[position_++]));
}

::jxx::lang::jint BufferedInputStream::read(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    ensureOpen_();
    IOHelper::checkBounds(buffer, offset, length);

    if (length == 0) {
        return 0;
    }

    ::jxx::lang::jint total = 0;

    while (total < length) {
        if (position_ >= count_ && fill_() < 0) {
            break;
        }

        const auto amount =
            std::min(
                length - total,
                count_ - position_);

        for (::jxx::lang::jint index = 0;
             index < amount;
             ++index) {
            (*buffer)[offset + total + index] =
                (*buffer_)[position_ + index];
        }

        position_ += amount;
        total += amount;
    }

    return total == 0 ? -1 : total;
}

::jxx::lang::jlong BufferedInputStream::skip(
    ::jxx::lang::jlong count) {

    ensureOpen_();

    if (count <= 0) {
        return 0;
    }

    const auto buffered =
        static_cast<::jxx::lang::jlong>(count_ - position_);

    if (buffered > 0) {
        const auto amount = std::min(count, buffered);
        position_ += static_cast<::jxx::lang::jint>(amount);
        return amount;
    }

    if (markPosition_ < 0) {
        return in_->skip(count);
    }

    if (fill_() < 0) {
        return 0;
    }

    const auto amount =
        std::min(
            count,
            static_cast<::jxx::lang::jlong>(count_ - position_));

    position_ += static_cast<::jxx::lang::jint>(amount);
    return amount;
}

::jxx::lang::jint BufferedInputStream::available() {
    ensureOpen_();
    return count_ - position_ + in_->available();
}

void BufferedInputStream::close() {
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

void BufferedInputStream::mark(
    ::jxx::lang::jint readLimit) {

    markLimit_ = readLimit;
    markPosition_ = position_;
}

void BufferedInputStream::reset() {
    ensureOpen_();

    if (markPosition_ < 0) {
        throw IOException();
    }

    position_ = markPosition_;
}

::jxx::lang::jbool
BufferedInputStream::markSupported() const {
    return true;
}

} // namespace jxx::io
