#include "io/jxx.io.BufferedReader.h"

#include <algorithm>
#include <cstdint>
#include <string>

#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

BufferedReader::BufferedReader(
    const ::jxx::Ptr<Reader>& input)
    : BufferedReader(input, 8192) {
}

BufferedReader::BufferedReader(
    const ::jxx::Ptr<Reader>& input,
    ::jxx::lang::jint size)
    : Super()
    , in_(input) {

    if (input == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (size <= 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }

    buffer_ = ::jxx::NEW<::jxx::lang::CharArrayType>(
        static_cast<std::uint32_t>(size));
}

::jxx::lang::jint BufferedReader::fill() {
    position_ = 0;
    count_ = in_->read(
        buffer_,
        0,
        static_cast<::jxx::lang::jint>(buffer_->length));
    return count_;
}

::jxx::lang::jint BufferedReader::read() {
    return lock->synchronized([&]() -> ::jxx::lang::jint {
        if (position_ >= count_ && fill() < 0) {
            return -1;
        }
        return static_cast<::jxx::lang::jint>((*buffer_)[position_++]);
    });
}

::jxx::lang::jint BufferedReader::read(
    const ::jxx::lang::CharArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    return lock->synchronized([&]() -> ::jxx::lang::jint {
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

        ::jxx::lang::jint total = 0;
        while (total < length) {
            const auto value = read();
            if (value < 0) {
                break;
            }
            (*buffer)[offset + total++] =
                static_cast<::jxx::lang::jchar>(value);
        }
        return total == 0 ? -1 : total;
    });
}

::jxx::Ptr<::jxx::lang::String> BufferedReader::readLine() {
    return lock->synchronized([&]() -> ::jxx::Ptr<::jxx::lang::String> {
        std::u16string line;
        for (;;) {
            const auto value = read();
            if (value < 0) {
                return line.empty()
                    ? nullptr
                    : ::jxx::NEW<::jxx::lang::String>(line);
            }
            if (value == '\n') {
                break;
            }
            if (value == '\r') {
                const auto next = read();
                if (next >= 0 && next != '\n') {
                    --position_;
                }
                break;
            }
            line.push_back(static_cast<char16_t>(value));
        }
        return ::jxx::NEW<::jxx::lang::String>(line);
    });
}

::jxx::lang::jlong BufferedReader::skip(::jxx::lang::jlong count) {
    return lock->synchronized([&]() -> ::jxx::lang::jlong {
        if (count < 0) {
            throw ::jxx::lang::IllegalArgumentException();
        }
        ::jxx::lang::jlong total = 0;
        while (total < count && read() >= 0) {
            ++total;
        }
        return total;
    });
}

::jxx::lang::jbool BufferedReader::ready() {
    return lock->synchronized([&]() -> ::jxx::lang::jbool {
        return position_ < count_ || in_->ready();
    });
}

::jxx::lang::jbool BufferedReader::markSupported() const {
    return true;
}

void BufferedReader::mark(::jxx::lang::jint readAheadLimit) {
    lock->synchronized([&] {
        if (readAheadLimit < 0) {
            throw ::jxx::lang::IllegalArgumentException();
        }
        mark_ = position_;
    });
}

void BufferedReader::reset() {
    lock->synchronized([&] {
        if (mark_ < 0) {
            throw IOException();
        }
        position_ = mark_;
    });
}

void BufferedReader::close() {
    lock->synchronized([&] {
        if (in_ == nullptr) {
            return;
        }
        buffer_.reset();
        in_->close();
        in_.reset();
    });
}

} // namespace jxx::io
