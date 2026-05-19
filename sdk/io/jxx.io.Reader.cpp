
#pragma once
#include "io/jxx.io.IOException.h"
#include "jxx.io.Reader.h"

namespace jxx {
    namespace io {

        int Reader::read(CharArray& buf, int off, int len) {
            if (off < 0 || len < 0) throw IOException("Reader.read: invalid args");
            int i = 0; for (; i < len; ++i) {
                int c = read();
                if (c < 0) break;
                buf[off + i] = (jxx::lang::jchar)c;
            }
            return (i == 0) ? -1 : i;
        }
        void Reader::close() {}
        long Reader::skip(long n) { long r = 0; for (; r < n; ++r) { if (read() < 0) break; } return r; }
        bool Reader::ready() { return false; }
        bool Reader::markSupported() { return false; }
        void Reader::mark(int) { throw IOException("mark not supported"); }
        void Reader::reset() { throw IOException("reset not supported"); }
    }
}