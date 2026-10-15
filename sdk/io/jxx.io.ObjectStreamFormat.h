#pragma once

#include <cstdint>

namespace jxx::io::serialization {
inline constexpr std::uint32_t STREAM_MAGIC = 0x4A58584FU;
inline constexpr std::uint8_t STREAM_MAJOR_VERSION = 1U;
inline constexpr std::uint8_t STREAM_MINOR_VERSION = 0U;
inline constexpr std::uint16_t STREAM_FLAGS = 0x0004U;
inline constexpr std::uint64_t FIRST_HANDLE = 1U;

enum class ArrayElementKind : std::uint8_t {
    Boolean = 0x01, Byte = 0x02, Char = 0x03, Short = 0x04,
    Int = 0x05, Long = 0x06, Float = 0x07, Double = 0x08,
    Object = 0x09, TypedObject = 0x0A
};

enum class Tag : std::uint8_t {
    Null = 0x00,
    Reference = 0x01,
    Reset = 0x02,
    Exception = 0x03,
    NewObject = 0x10,
    NewString = 0x11,
    NewArray = 0x12,
    NewEnum = 0x13,
    NewClass = 0x14,
    ClassDescriptor = 0x20,
    ClassDescriptorRef = 0x21,
    EndClassDescriptor = 0x22,
    EndObject = 0x23,
    EndArray = 0x24,
    EndCustomData = 0x25,
    BlockData = 0x30,
    LongBlockData = 0x31,
    Extension = 0x7F
};
} // namespace jxx::io::serialization
