#pragma once

#include "io/jxx.io.FilterOutputStream.h"
#include "util/zip/jxx.util.zip.Deflater.h"

namespace jxx::util::zip {

class DeflaterOutputStream
    : public ::jxx::lang::ClassBase<DeflaterOutputStream, ::jxx::io::FilterOutputStream> {
public:
    using JxxSuper = ::jxx::io::FilterOutputStream;
    using Super = ::jxx::lang::ClassBase<DeflaterOutputStream, JxxSuper>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    explicit DeflaterOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output);
    DeflaterOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output, const ::jxx::Ptr<Deflater>& deflater);
    DeflaterOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output, ::jxx::lang::jbool syncFlush);
    DeflaterOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output, const ::jxx::Ptr<Deflater>& deflater, ::jxx::lang::jint size);
    DeflaterOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output, const ::jxx::Ptr<Deflater>& deflater, ::jxx::lang::jint size, ::jxx::lang::jbool syncFlush);
    ~DeflaterOutputStream() override;

    void write(::jxx::lang::jint value) override;
    void write(const ::jxx::lang::ByteArray& buffer) override;
    void write(const ::jxx::lang::ByteArray& buffer, ::jxx::lang::jint offset, ::jxx::lang::jint length) override;
    void finish();
    void close() override;
    void flush() override;

protected:
    void deflate();
    ::jxx::Ptr<Deflater> def;
    ::jxx::lang::ByteArray buf;

private:
    void ensureOpen_() const;
    bool usesDefaultDeflater_ = false;
    bool syncFlush_ = false;
    bool closed_ = false;
    bool finished_ = false;
};

} // namespace jxx::util::zip
