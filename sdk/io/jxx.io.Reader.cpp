#pragma once
#include "io/jxx.io.IOException.h"
#include "jxx.io.Reader.h"

namespace jxx {
    namespace io {

        int Reader::read(const jxx::lang::CharArray buf, jxx::lang::jint off, jxx::lang::jint len) {
            if (off < 0 || len < 0) throw IOException("Reader.read: invalid args");
            int i = 0; for (; i < len; ++i) {
                int c = read();
                if (c < 0) break;
                (*buf)[off + i] = (jxx::lang::jchar)c;
            }
            return (i == 0) ? -1 : i;
        }
        void Reader::close() {}
        jxx::lang::jlong Reader::skip(jxx::lang::jlong n) { jxx::lang::jlong r = 0; for (; r < n; ++r) { if (read() < 0) break; } return r; }
        jxx::lang::jbool Reader::ready() { return false; }
        jxx::lang::jbool Reader::markSupported() { return false; }
        void Reader::mark(jxx::lang::jint) { throw IOException("mark not supported"); }
        void Reader::reset() { throw IOException("reset not supported"); }
    }
}