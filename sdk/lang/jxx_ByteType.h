// byte.hpp
#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace jxx::lang {

// Java's signed byte (−128..127)
using jbyte = std::int8_t;

// Raw octet (0..255) – recommended for I/O and hashing
using ubyte = std::uint8_t;

// Closest analog to Java's `byte[]`
using ByteArray = std::vector<ubyte>;

// Helpers
inline ByteArray toBytes(const std::string& s) {
    return ByteArray{reinterpret_cast<const ubyte*>(s.data()),
                     reinterpret_cast<const ubyte*>(s.data()) + s.size()};
}

inline std::string toString(const ByteArray& bytes) {
    // binary-safe: std::string can hold NUL bytes
    return std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}

} // namespace jxx