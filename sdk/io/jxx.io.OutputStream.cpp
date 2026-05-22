#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "jxx.io.OutputStream.h"

namespace jxx::io {

void OutputStream::checkBounds_(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) {
    if (!b) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("b"));
    if (off < 0 || len < 0 || (std::uint32_t)(off + len) > b->length) {
        throw jxx::lang::IndexOutOfBoundsException(jxx::NEW<jxx::lang::String>("off/len"));
    }
}

void OutputStream::write(jxx::Ptr<ByteArray> b) {
    if (!b) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("b"));
    write(b, 0, (jxx::lang::jint)b->length);
}

void OutputStream::write(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) {
    checkBounds_(b, off, len);
    for (jxx::lang::jint i = 0; i < len; ++i) {
        write(((jxx::lang::jint)(*b)[off + i]) & 0xFF);
    }
}

void OutputStream::flush() {}
void OutputStream::close() {}

} // namespace jxx::io
