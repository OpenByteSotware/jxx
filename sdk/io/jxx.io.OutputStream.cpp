#include "io/jxx.io.OutputStream.h"

#include "io/jxx.io.IOHelper.h"

namespace jxx::io {

void OutputStream::write(
    const ::jxx::lang::ByteArray& buffer) {

    synchronized([&] {
        IOHelper::checkBounds(
            buffer,
            0,
            static_cast<::jxx::lang::jint>(buffer->length));
        write(
            buffer,
            0,
            static_cast<::jxx::lang::jint>(buffer->length));
    });
}

void OutputStream::write(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    synchronized([&] {
        IOHelper::checkBounds(buffer, offset, length);
        for (::jxx::lang::jint index = 0;
             index < length;
             ++index) {
            write(
                static_cast<::jxx::lang::jint>(
                    static_cast<unsigned char>(
                        (*buffer)[offset + index])));
        }
    });
}

void OutputStream::flush() {
}

void OutputStream::close() {
}

} // namespace jxx::io
