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

void BufferedReader::ensureOpen_() const {
    if (in_ == nullptr) {
        throw IOException();
    }
}

::jxx::lang::jint BufferedReader::fill_() {
    ensureOpen_();

    ::jxx::lang::jint destination = 0;
    if (markedPosition_ >= 0) {
        const auto consumedSinceMark = position_ - markedPosition_;
        if (consumedSinceMark >= readAheadLimit_) {
            markedPosition_ = -1;
            readAheadLimit_ = 0;
        } else {
            if (readAheadLimit_ >
                static_cast<::jxx::lang::jint>(buffer_->length)) {
                auto expanded = ::jxx::NEW<::jxx::lang::CharArrayType>(
                    static_cast<std::uint32_t>(readAheadLimit_));
                for (::jxx::lang::jint index = markedPosition_;
                     index < position_;
                     ++index) {
                    (*expanded)[index - markedPosition_] = (*buffer_)[index];
                }
                buffer_ = expanded;
            } else {
                for (::jxx::lang::jint index = markedPosition_;
                     index < position_;
                     ++index) {
                    (*buffer_)[index - markedPosition_] = (*buffer_)[index];
                }
            }
            destination = consumedSinceMark;
            position_ = consumedSinceMark;
            markedPosition_ = 0;
        }
    }

    const auto available =
        static_cast<::jxx::lang::jint>(buffer_->length) - destination;
    const auto amount = in_->read(buffer_, destination, available);
    if (amount > 0) {
        count_ = destination + amount;
        if (markedPosition_ < 0) {
            position_ = 0;
        }
    } else {
        count_ = destination;
    }
    return amount;
}

::jxx::lang::jint BufferedReader::read() {
    return lock->synchronized([&]() -> ::jxx::lang::jint {
        ensureOpen_();
        if (position_ >= count_ && fill_() < 0) {
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
        ensureOpen_();
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
        ensureOpen_();
        return position_ < count_ || in_->ready();
    });
}

::jxx::lang::jbool BufferedReader::markSupported() const {
    return true;
}

void BufferedReader::mark(::jxx::lang::jint readAheadLimit) {
    lock->synchronized([&] {
        ensureOpen_();
        if (readAheadLimit < 0) {
            throw ::jxx::lang::IllegalArgumentException();
        }
        readAheadLimit_ = readAheadLimit;
        markedPosition_ = position_;
    });
}

void BufferedReader::reset() {
    lock->synchronized([&] {
        ensureOpen_();
        if (markedPosition_ < 0) {
            throw IOException();
        }
        position_ = markedPosition_;
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
        position_ = 0;
        count_ = 0;
        markedPosition_ = -1;
        readAheadLimit_ = 0;
    });
}

} // namespace jxx::io
