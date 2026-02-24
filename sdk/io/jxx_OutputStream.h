// output_stream.hpp
#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>
#include <algorithm>

namespace jxx::io {

struct IOException : public std::ios_base::failure {
    explicit IOException(const std::string& msg) : std::ios_base::failure(msg) {}
};

class OutputStream {
public:
    OutputStream() = default;
    virtual ~OutputStream() = default;

    OutputStream(const OutputStream&) = delete;
    OutputStream& operator=(const OutputStream&) = delete;
    OutputStream(OutputStream&&) = default;
    OutputStream& operator=(OutputStream&&) = default;

    // --- Java-like API ---

    // write(int b) — writes the low 8 bits of the int
    virtual void write(int b) {
        std::uint8_t one = static_cast<std::uint8_t>(b & 0xFF);
        write(&one, 0, 1);
    }

    // write(byte[] b)
    virtual void write(const std::vector<std::uint8_t>& b) {
        if (!b.empty()) write(b.data(), 0, b.size());
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

} // namespace jxx::io