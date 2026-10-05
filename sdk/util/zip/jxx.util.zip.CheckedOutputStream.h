#pragma once

#include "io/jxx.io.FilterOutputStream.h"
#include "util/zip/jxx.util.zip.Checksum.h"

namespace jxx::util::zip {

class CheckedOutputStream final
    : public ::jxx::lang::ClassBase<CheckedOutputStream, ::jxx::io::FilterOutputStream> {
public:
    using JxxSuper = ::jxx::io::FilterOutputStream;
    using Super = ::jxx::lang::ClassBase<CheckedOutputStream, JxxSuper>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    CheckedOutputStream(
        const ::jxx::Ptr<::jxx::io::OutputStream>& output,
        const ::jxx::Ptr<Checksum>& checksum);

    void write(::jxx::lang::jint value) override;
    void write(const ::jxx::lang::ByteArray& buffer) override;
    void write(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;

    ::jxx::Ptr<Checksum> getChecksum() const noexcept;

private:
    ::jxx::Ptr<Checksum> checksum_;
};

} // namespace jxx::util::zip
