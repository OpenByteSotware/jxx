#pragma once

#include <cstdint>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/zip/jxx.util.zip.Checksum.h"

namespace jxx::nio {
class ByteBuffer;
}

namespace jxx::util::zip {

class Adler32 final
    : public ::jxx::lang::ClassBase<Adler32, ::jxx::lang::Object, Checksum> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<Adler32, JxxSuper, Checksum>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    Adler32() noexcept;

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
    static constexpr std::uint32_t MOD_ADLER = 65521U;
    std::uint32_t adler_ = 1U;
};

} // namespace jxx::util::zip
