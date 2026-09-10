#pragma once

#include "io/jxx.io.FilterInputStream.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace jxx::io {

class BufferedInputStream
    : public ::jxx::lang::ClassBase<
          BufferedInputStream,
          FilterInputStream> {
public:
    using JxxSuper = FilterInputStream;

    using Super =
        ::jxx::lang::ClassBase<
            BufferedInputStream,
            JxxSuper>;

    explicit BufferedInputStream(
        const ::jxx::Ptr<InputStream>& input);

    BufferedInputStream(
        const ::jxx::Ptr<InputStream>& input,
        ::jxx::lang::jint size);

    ~BufferedInputStream() override;

    ::jxx::lang::jint read() override;

    ::jxx::lang::jint read(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;

    ::jxx::lang::jlong skip(
        ::jxx::lang::jlong count) override;

    ::jxx::lang::jint available() override;
    void close() override;

    void mark(
        ::jxx::lang::jint readLimit) override;

    void reset() override;

    ::jxx::lang::jbool
    markSupported() const override;

private:
    static ::jxx::lang::jint validateBufferSize_(
        ::jxx::lang::jint size);

    void ensureOpen_() const;
    ::jxx::lang::jint fill_();

    ::jxx::lang::ByteArray buffer_;
    ::jxx::lang::jint position_ = 0;
    ::jxx::lang::jint count_ = 0;
    ::jxx::lang::jint markPosition_ = -1;
    ::jxx::lang::jint markLimit_ = 0;
    ::jxx::lang::jbool closed_ = false;
};

} // namespace jxx::io
