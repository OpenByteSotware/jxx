
#pragma once
#include <algorithm>
#include <limits>
#include <cstring>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.IOException.h"
#include "io/jxx.io.Closeable.h"

using namespace jxx::lang;

namespace jxx { namespace io {

    class InputStream : public jxx::lang::Object, public jxx::io::Closeable {
    public:
        virtual ~InputStream() = default;
        virtual int read() = 0;
        virtual int read(ByteArray& b, int off, int len) {
            if (b.length < (size_t)off + (size_t)len) throw IOException("IndexOutOfBounds in InputStream.read");
            if (len == 0) return 0;
            int c = read();
            if (c == -1) return -1;
            b[off] = (std::uint8_t)c;
            int i = 1;
            for (; i < len; ++i) { c = read(); if (c == -1) break; b[(jbyte)(off+i)] = (jbyte)c; }
            return i;
        }
        virtual int read(ByteArray& b) { return read(b, 0, (int)b.length); }
        virtual int skip(int n) {
            int rem = n; ByteArray tmp(2048);
            while (rem > 0) { auto r = read(tmp, 0, (int)std::min<int>(rem, (int)tmp.length)); if (r < 0) break; rem -= r; if (r == 0) break; }
            return n - rem;
        }

        virtual int available() { return 0; }
        virtual void close() override {}
        virtual void mark(int) {}
        virtual void reset() { throw IOException("mark/reset not supported"); }
        virtual bool markSupported() { return false; }
    };
}}
