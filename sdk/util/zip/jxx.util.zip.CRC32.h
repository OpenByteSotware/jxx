#pragma once

#include <array>
#include <cstdint>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/zip/jxx.util.zip.Checksum.h"

namespace jxx::nio {
class ByteBuffer;
}

namespace jxx::util::zip {

class CRC32 final
    : public ::jxx::lang::ClassBase<
          CRC32,
          ::jxx::lang::Object,
          Checksum> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<CRC32, JxxSuper, Checksum>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    CRC32() noexcept;

    void update(::jxx::lang::jint value) override;

    void update(const ::jxx::lang::ByteArray& buffer);

    void update(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;

    void update(const ::jxx::Ptr<::jxx::nio::ByteBuffer>& buffer);

    ::jxx::lang::jlong getValue() const noexcept override;
    void reset() noexcept override;

private:
    static const std::array<std::uint32_t, 256U>& table() noexcept;
    void updateByte(std::uint8_t value) noexcept;

    std::uint32_t crc_ = 0xFFFFFFFFU;
};

} // namespace jxx::util::zip
