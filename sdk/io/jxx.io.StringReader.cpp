#include <algorithm>
#include "io/jxx.io.StringReader.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

StringReader::StringReader(
    const ::jxx::Ptr<::jxx::lang::String>& value)
    : Super()
    , value_(value) {

    if (value == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

void StringReader::ensureOpen() const {
    if (value_ == nullptr) {
        throw IOException();
    }
}

::jxx::lang::jint StringReader::read() {
    return lock->synchronized([&]() -> ::jxx::lang::jint {
        ensureOpen();
        return position_ >= value_->length()
            ? -1
            : static_cast<::jxx::lang::jint>(
                  value_->charAt(position_++));
    });
}

::jxx::lang::jint StringReader::read(
    const ::jxx::lang::CharArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    return lock->synchronized([&]() -> ::jxx::lang::jint {
        ensureOpen();
        if (buffer == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        if (offset < 0 || length < 0 ||
            offset > static_cast<::jxx::lang::jint>(buffer->length) - length) {
            throw ::jxx::lang::IndexOutOfBoundsException();
        }
        if (length == 0) {
            return 0;
        }
        if (position_ >= value_->length()) {
            return -1;
        }

        const auto count = std::min(
            length,
            value_->length() - position_);
        for (::jxx::lang::jint index = 0;
             index < count;
             ++index) {
            (*buffer)[offset + index] =
                value_->charAt(position_ + index);
        }
        position_ += count;
        return count;
    });
}

::jxx::lang::jlong StringReader::skip(
    ::jxx::lang::jlong count) {

    return lock->synchronized([&]() -> ::jxx::lang::jlong {
        ensureOpen();
        const auto amount = std::max<::jxx::lang::jlong>(
            -position_,
            std::min<::jxx::lang::jlong>(
                count,
                value_->length() - position_));
        position_ += static_cast<::jxx::lang::jint>(amount);
        return amount;
    });
}

::jxx::lang::jbool StringReader::ready() {
    return lock->synchronized([&]() -> ::jxx::lang::jbool {
        ensureOpen();
        return true;
    });
}

::jxx::lang::jbool StringReader::markSupported() const {
    return true;
}

void StringReader::mark(
    ::jxx::lang::jint readAheadLimit) {

    lock->synchronized([&] {
        ensureOpen();
        if (readAheadLimit < 0) {
            throw ::jxx::lang::IllegalArgumentException();
        }
        mark_ = position_;
    });
}

void StringReader::reset() {
    lock->synchronized([&] {
        ensureOpen();
        position_ = mark_;
    });
}

void StringReader::close() {
    lock->synchronized([&] {
        value_.reset();
    });
}

} // namespace jxx::io
