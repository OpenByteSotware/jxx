#pragma once

#include "io/jxx.io.FilterInputStream.h"
#include "util/zip/jxx.util.zip.Checksum.h"

namespace jxx::util::zip {

class CheckedInputStream final
    : public ::jxx::lang::ClassBase<CheckedInputStream, ::jxx::io::FilterInputStream> {
public:
    using JxxSuper = ::jxx::io::FilterInputStream;
    using Super = ::jxx::lang::ClassBase<CheckedInputStream, JxxSuper>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    CheckedInputStream(
        const ::jxx::Ptr<::jxx::io::InputStream>& input,
        const ::jxx::Ptr<Checksum>& checksum);

    ::jxx::lang::jint read() override;
    ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer) override;
    ::jxx::lang::jint read(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;
    ::jxx::lang::jlong skip(::jxx::lang::jlong count) override;

    ::jxx::Ptr<Checksum> getChecksum() const noexcept;

private:
    ::jxx::Ptr<Checksum> checksum_;
};

} // namespace jxx::util::zip
