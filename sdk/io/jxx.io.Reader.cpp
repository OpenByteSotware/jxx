#include "io/jxx.io.Reader.h"

#include <algorithm>
#include <cstdint>

#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::io {

namespace {

void checkBounds(
    const ::jxx::lang::CharArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    if (buffer == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    const auto bufferLength =
        static_cast<::jxx::lang::jint>(buffer->length);

    if (offset < 0 ||
        length < 0 ||
        offset > bufferLength - length) {
        throw ::jxx::lang::IndexOutOfBoundsException();
    }
}

} // namespace

Reader::Reader()
    : Super()
    , lock(::jxx::NEW<::jxx::lang::Object>()) {
}

Reader::Reader(
    const ::jxx::Ptr<::jxx::lang::Object>& lockObject)
    : Super()
    , lock(lockObject) {

    if (lock == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::lang::jint Reader::read() {
    const auto buffer =
        ::jxx::NEW<::jxx::lang::CharArrayType>(1);

    const auto count = read(buffer, 0, 1);

    if (count < 0) {
        return -1;
    }

    return static_cast<::jxx::lang::jint>((*buffer)[0]);
}

::jxx::lang::jint Reader::read(
    const ::jxx::lang::CharArray& buffer) {

    if (buffer == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    const auto length =
        static_cast<::jxx::lang::jint>(buffer->length);

    checkBounds(buffer, 0, length);
    return read(buffer, 0, length);
}

::jxx::lang::jlong Reader::skip(
    ::jxx::lang::jlong count) {

    if (count < 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }

    const auto buffer =
        ::jxx::NEW<::jxx::lang::CharArrayType>(8192);

    ::jxx::lang::jlong total = 0;

    while (total < count) {
        const auto requested =
            static_cast<::jxx::lang::jint>(
                std::min<::jxx::lang::jlong>(
                    count - total,
                    8192));

        const auto readCount =
            read(buffer, 0, requested);

        if (readCount < 0) {
            break;
        }

        total += readCount;
    }

    return total;
}

::jxx::lang::jbool Reader::ready() {
    return false;
}

::jxx::lang::jbool Reader::markSupported() const {
    return false;
}

void Reader::mark(
    ::jxx::lang::jint readAheadLimit) {
    (void)readAheadLimit;
    throw IOException();
}

void Reader::reset() {
    throw IOException();
}

} // namespace jxx::io
