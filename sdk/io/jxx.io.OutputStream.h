// output_stream.hpp
#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.IOException.h"
using namespace std;

namespace jxx::io {

class OutputStream {
public:
    OutputStream() = default;
    virtual ~OutputStream() = default;

    OutputStream(const OutputStream&) = delete;
    OutputStream& operator=(const OutputStream&) = delete;
    OutputStream(OutputStream&&) = default;
    OutputStream& operator=(OutputStream&&) = default;

    virtual void write(int b) {
        std::uint8_t one = static_cast<std::uint8_t>(b & 0xFF);
        write(&one, 0, 1);
    }

    virtual void write(const ByteArray& b, int off, int len) {
        if (b.size() != 0) {
            write(b.data(), 0, b.size());
        }
    }

    // write(byte[] b, int off, int len)
    virtual void write(const std::uint8_t* b, std::size_t off, std::size_t len) = 0;

    // flush()
    virtual void flush() {}

    // close()
    virtual void close() {}

    // Convenience overloads (C++ only)
    void write(const std::string& s) {
        if (!s.empty()) {
            const auto* p = reinterpret_cast<const std::uint8_t*>(s.data());
            write(p, 0, s.size());
        }
    }

    void write(const std::uint8_t* data, std::size_t len) {
        if (len) write(data, 0, len);
    }

    // NullOutputStream (like Java 11’s OutputStream.nullOutputStream())
    static OutputStream& nullStream();

protected:
    // Protected helper to throw I/O failures
    [[noreturn]] static void fail(const std::string& msg) {
        throw IOException(msg);
    }
};

}