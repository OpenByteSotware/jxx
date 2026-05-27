#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "io/jxx.io.IOException.h"
#include "io/jxx.io.InputStream.h"

namespace jxx::io {

    void InputStream::checkBounds_(const jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len) {
        if (!b) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("b"));
        if (off < 0 || len < 0 || (std::uint32_t)(off + len) > b->length) {
            throw jxx::lang::IndexOutOfBoundsException(jxx::NEW<jxx::lang::String>("off/len"));
        }
    }

    jxx::lang::jint InputStream::read(const jxx::lang::ByteArray b) {
        if (!b) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("b"));
        return read(b, 0, (jxx::lang::jint)b->length);
    }

    jxx::lang::jint InputStream::read(const jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len) {
        checkBounds_(b, off, len);
        if (len == 0) return 0;

        jxx::lang::jint c = read();
        if (c < 0) return -1;

        (*b)[off] = (jxx::lang::jbyte)(c & 0xFF);

        jxx::lang::jint i = 1;
        while (i < len) {
            c = read();
            if (c < 0) break;
            (*b)[off + i] = (jxx::lang::jbyte)(c & 0xFF);
            ++i;
        }
        return i;
    }

    jxx::lang::jlong InputStream::skip(jxx::lang::jlong n) {
        if (n <= 0) return 0;

        jxx::lang::jlong skipped = 0;
        auto tmp = jxx::NEW<jxx::lang::ByteArrayType>(512);

        while (skipped < n) {
            jxx::lang::jint toRead = (jxx::lang::jint)((n - skipped) < 512 ? (n - skipped) : 512);
            jxx::lang::jint r = read(tmp, 0, toRead);
            if (r < 0) break;
            skipped += r;
        }
        return skipped;
    }

    jxx::lang::jint InputStream::available() {
        return 0;
    }

    void InputStream::close() {
        // no-op
    }

    void InputStream::mark(jxx::lang::jint) {}
    void InputStream::reset() {
        throw IOException(jxx::NEW<jxx::lang::String>("reset not supported"));
    }
    jxx::lang::jbool InputStream::markSupported() {
        return false;
    }

} // namespace jxx::io