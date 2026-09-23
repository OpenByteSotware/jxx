#include <algorithm>
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.IOHelper.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

::jxx::lang::jint InputStream::read(
    const ::jxx::lang::ByteArray& buffer) {

    return synchronized([&]() -> ::jxx::lang::jint {
        IOHelper::checkBounds(
            buffer,
            0,
            static_cast<::jxx::lang::jint>(buffer->length));
        return read(
            buffer,
            0,
            static_cast<::jxx::lang::jint>(buffer->length));
    });
}

::jxx::lang::jint InputStream::read(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    return synchronized([&]() -> ::jxx::lang::jint {
        IOHelper::checkBounds(buffer, offset, length);
        if (length == 0) {
            return 0;
        }

        auto value = read();
        if (value < 0) {
            return -1;
        }

        (*buffer)[offset] = static_cast<::jxx::lang::jbyte>(value);
        ::jxx::lang::jint count = 1;
        try {
            for (; count < length; ++count) {
                value = read();
                if (value < 0) {
                    break;
                }
                (*buffer)[offset + count] =
                    static_cast<::jxx::lang::jbyte>(value);
            }
        }
        catch (const IOException&) {
        }
        return count;
    });
}

::jxx::lang::jlong InputStream::skip(::jxx::lang::jlong count) {
    return synchronized([&]() -> ::jxx::lang::jlong {
        if (count <= 0) {
            return 0;
        }

        const auto temporary =
            ::jxx::NEW<::jxx::lang::ByteArrayType>(2048);
        ::jxx::lang::jlong total = 0;
        while (total < count) {
            const auto amount = static_cast<::jxx::lang::jint>(
                std::min<::jxx::lang::jlong>(count - total, 2048));
            const auto readCount = read(temporary, 0, amount);
            if (readCount < 0) {
                break;
            }
            total += readCount;
        }
        return total;
    });
}

::jxx::lang::jint InputStream::available() {
    return 0;
}

void InputStream::close() {
}

void InputStream::mark(::jxx::lang::jint readLimit) {
    (void)readLimit;
}

void InputStream::reset() {
    throw IOException(
        ::jxx::NEW<::jxx::lang::String>("reset not supported"));
}

::jxx::lang::jbool InputStream::markSupported() const {
    return false;
}

} // namespace jxx::io
