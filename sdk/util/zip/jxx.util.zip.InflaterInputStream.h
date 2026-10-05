#pragma once

#include "io/jxx.io.FilterInputStream.h"
#include "util/zip/jxx.util.zip.Inflater.h"

namespace jxx::util::zip {

class InflaterInputStream
    : public ::jxx::lang::ClassBase<InflaterInputStream, ::jxx::io::FilterInputStream> {
public:
    using JxxSuper = ::jxx::io::FilterInputStream;
    using Super = ::jxx::lang::ClassBase<InflaterInputStream, JxxSuper>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    explicit InflaterInputStream(const ::jxx::Ptr<::jxx::io::InputStream>& input);
    InflaterInputStream(const ::jxx::Ptr<::jxx::io::InputStream>& input, const ::jxx::Ptr<Inflater>& inflater);
    InflaterInputStream(const ::jxx::Ptr<::jxx::io::InputStream>& input, const ::jxx::Ptr<Inflater>& inflater, ::jxx::lang::jint size);
    ~InflaterInputStream() override;

    ::jxx::lang::jint read() override;
    ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer) override;
    ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer, ::jxx::lang::jint offset, ::jxx::lang::jint length) override;
    ::jxx::lang::jlong skip(::jxx::lang::jlong count) override;
    ::jxx::lang::jint available() override;
    void close() override;
    ::jxx::lang::jbool markSupported() const override;
    void mark(::jxx::lang::jint readLimit) override;
    void reset() override;

protected:
    virtual void fill();
    void resetInflationState_() noexcept;
    ::jxx::Ptr<Inflater> inf;
    ::jxx::lang::ByteArray buf;
    ::jxx::lang::jint len = 0;

private:
    void ensureOpen_() const;
    bool usesDefaultInflater_ = false;
    bool closed_ = false;
    bool eof_ = false;
};

} // namespace jxx::util::zip
