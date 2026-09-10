#include "io/jxx.io.PushbackReader.h"

#include <algorithm>
#include <cstdint>

#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::io {

PushbackReader::PushbackReader(const ::jxx::Ptr<Reader>& input)
    : PushbackReader(input, 1) {
}

PushbackReader::PushbackReader(
    const ::jxx::Ptr<Reader>& input,
    ::jxx::lang::jint size)
    : Super(input)
    , buffer_(::jxx::NEW<::jxx::lang::CharArrayType>(
          static_cast<std::uint32_t>(validateSize_(size))))
    , position_(size) {
}

PushbackReader::~PushbackReader() = default;

::jxx::lang::jint PushbackReader::validateSize_(::jxx::lang::jint size) {
    if (size <= 0) throw ::jxx::lang::IllegalArgumentException();
    return size;
}

void PushbackReader::ensureOpen_() const {
    if (closed_ || buffer_ == nullptr || in_ == nullptr) throw IOException();
}

::jxx::lang::jint PushbackReader::read() {
    ensureOpen_();
    if (position_ < static_cast<::jxx::lang::jint>(buffer_->length)) {
        return static_cast<::jxx::lang::jint>((*buffer_)[position_++]);
    }
    return in_->read();
}

::jxx::lang::jint PushbackReader::read(
    const ::jxx::lang::CharArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {
    ensureOpen_();
    if (!buffer) throw ::jxx::lang::NullPointerException();
    if (offset < 0 || length < 0 ||
        offset > static_cast<::jxx::lang::jint>(buffer->length) - length) {
        throw ::jxx::lang::IndexOutOfBoundsException();
    }
    if (length == 0) return 0;

    const auto pushed = static_cast<::jxx::lang::jint>(buffer_->length) - position_;
    const auto copied = std::min(length, pushed);
    for (::jxx::lang::jint i = 0; i < copied; ++i) {
        (*buffer)[offset + i] = (*buffer_)[position_ + i];
    }
    position_ += copied;
    if (copied == length) return copied;
    const auto count = in_->read(buffer, offset + copied, length - copied);
    return count < 0 ? (copied == 0 ? -1 : copied) : copied + count;
}

void PushbackReader::unread(::jxx::lang::jint value) {
    ensureOpen_();
    if (position_ == 0) throw IOException();
    (*buffer_)[--position_] = static_cast<::jxx::lang::jchar>(value);
}

void PushbackReader::unread(const ::jxx::lang::CharArray& buffer) {
    if (!buffer) throw ::jxx::lang::NullPointerException();
    unread(buffer, 0, static_cast<::jxx::lang::jint>(buffer->length));
}

void PushbackReader::unread(
    const ::jxx::lang::CharArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {
    ensureOpen_();
    if (!buffer) throw ::jxx::lang::NullPointerException();
    if (offset < 0 || length < 0 ||
        offset > static_cast<::jxx::lang::jint>(buffer->length) - length) {
        throw ::jxx::lang::IndexOutOfBoundsException();
    }
    if (length > position_) throw IOException();
    position_ -= length;
    for (::jxx::lang::jint i = 0; i < length; ++i) {
        (*buffer_)[position_ + i] = (*buffer)[offset + i];
    }
}

::jxx::lang::jbool PushbackReader::ready() {
    ensureOpen_();
    return position_ < static_cast<::jxx::lang::jint>(buffer_->length) || in_->ready();
}

::jxx::lang::jlong PushbackReader::skip(::jxx::lang::jlong count) {
    ensureOpen_();
    if (count < 0) throw ::jxx::lang::IllegalArgumentException();
    const auto pushed = static_cast<::jxx::lang::jlong>(buffer_->length) - position_;
    const auto first = std::min(count, pushed);
    position_ += static_cast<::jxx::lang::jint>(first);
    return first == count ? first : first + in_->skip(count - first);
}

::jxx::lang::jbool PushbackReader::markSupported() const { return false; }
void PushbackReader::mark(::jxx::lang::jint readAheadLimit) {
    (void)readAheadLimit;
    throw IOException();
}
void PushbackReader::reset() { throw IOException(); }

void PushbackReader::close() {
    if (closed_) return;
    closed_ = true;
    buffer_.reset();
    if (in_ != nullptr) {
        in_->close();
        in_.reset();
    }
}

} // namespace jxx::io
