#include "io/jxx.io.BufferedWriter.h"

#include <cstdint>

#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

BufferedWriter::BufferedWriter(
    const ::jxx::Ptr<Writer>& output)
    : BufferedWriter(output, 8192) {
}

BufferedWriter::BufferedWriter(
    const ::jxx::Ptr<Writer>& output,
    ::jxx::lang::jint size)
    : Super(output)
    , output_(output)
    , buffer_(
          ::jxx::NEW<::jxx::lang::CharArrayType>(
              static_cast<std::uint32_t>(
                  validateBufferSize_(size)))) {

    if (output_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

BufferedWriter::~BufferedWriter() = default;

::jxx::lang::jint
BufferedWriter::validateBufferSize_(
    ::jxx::lang::jint size) {

    if (size <= 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }

    return size;
}

void BufferedWriter::ensureOpen_() const {
    if (closed_ || output_ == nullptr || buffer_ == nullptr) {
        throw IOException();
    }
}

void BufferedWriter::flushBuffer_() {
    ensureOpen_();

    if (count_ <= 0) {
        return;
    }

    output_->write(buffer_, 0, count_);
    count_ = 0;
}

void BufferedWriter::write(
    ::jxx::lang::jint value) {

    ensureOpen_();

    if (count_ ==
        static_cast<::jxx::lang::jint>(buffer_->length)) {
        flushBuffer_();
    }

    (*buffer_)[count_++] =
        static_cast<::jxx::lang::jchar>(value);
}

void BufferedWriter::write(
    const ::jxx::lang::CharArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    ensureOpen_();

    if (buffer == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    if (offset < 0 ||
        length < 0 ||
        offset > static_cast<::jxx::lang::jint>(buffer->length) - length) {
        throw ::jxx::lang::IndexOutOfBoundsException();
    }

    for (::jxx::lang::jint index = 0;
         index < length;
         ++index) {
        write(static_cast<::jxx::lang::jint>(
            (*buffer)[offset + index]));
    }
}

void BufferedWriter::write(
    const ::jxx::Ptr<::jxx::lang::String>& value,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    ensureOpen_();

    if (value == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    if (offset < 0 ||
        length < 0 ||
        offset > value->length() - length) {
        throw ::jxx::lang::IndexOutOfBoundsException();
    }

    for (::jxx::lang::jint index = 0;
         index < length;
         ++index) {
        write(static_cast<::jxx::lang::jint>(
            value->charAt(offset + index)));
    }
}

void BufferedWriter::newLine() {
    JxxSuper::write(
        static_cast<::jxx::lang::jint>('\n'));
}

void BufferedWriter::flush() {
    ensureOpen_();
    flushBuffer_();
    output_->flush();
}

void BufferedWriter::close() {
    if (closed_) {
        return;
    }

    try {
        flushBuffer_();
        output_->flush();
    }
    catch (...) {
        try {
            output_->close();
        }
        catch (...) {
        }

        closed_ = true;
        output_.reset();
        buffer_.reset();
        throw;
    }

    output_->close();
    closed_ = true;
    output_.reset();
    buffer_.reset();
}

} // namespace jxx::io
