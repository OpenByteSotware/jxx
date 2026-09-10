#pragma once

#include "io/jxx.io.FilterInputStream.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace jxx::io {

class PushbackInputStream final
    : public ::jxx::lang::ClassBase<
          PushbackInputStream,
          FilterInputStream> {
public:
    using JxxSuper = FilterInputStream;

    using Super =
        ::jxx::lang::ClassBase<
            PushbackInputStream,
            JxxSuper>;

    explicit PushbackInputStream(
        const ::jxx::Ptr<InputStream>& input);

    PushbackInputStream(
        const ::jxx::Ptr<InputStream>& input,
        ::jxx::lang::jint size);

    ~PushbackInputStream() override;

    ::jxx::lang::jint read() override;

    ::jxx::lang::jint read(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;

    void unread(
        ::jxx::lang::jint value);

    void unread(
        const ::jxx::lang::ByteArray& buffer);

    void unread(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length);

    ::jxx::lang::jint available() override;

    ::jxx::lang::jlong skip(
        ::jxx::lang::jlong count) override;

    ::jxx::lang::jbool
    markSupported() const override;

    void mark(
        ::jxx::lang::jint readLimit) override;

    void reset() override;

    void close() override;

private:
    static ::jxx::lang::jint validateBufferSize_(
        ::jxx::lang::jint size);

    void ensureOpen() const;

    ::jxx::lang::ByteArray buffer_;
    ::jxx::lang::jint position_ = 0;
    ::jxx::lang::jbool closed_ = false;
};

} // namespace jxx::io
