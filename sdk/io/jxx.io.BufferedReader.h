#pragma once

#include "io/jxx.io.Reader.h"

namespace jxx::lang { class String; }

namespace jxx::io {

class BufferedReader final
    : public ::jxx::lang::ClassBase<BufferedReader, Reader> {
public:
    using JxxSuper = Reader;
    using Super = ::jxx::lang::ClassBase<BufferedReader, JxxSuper>;

    explicit BufferedReader(const ::jxx::Ptr<Reader>& input);
    BufferedReader(
        const ::jxx::Ptr<Reader>& input,
        ::jxx::lang::jint size);

    ::jxx::lang::jint read() override;
    ::jxx::lang::jint read(
        const ::jxx::lang::CharArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;
    ::jxx::Ptr<::jxx::lang::String> readLine();
    ::jxx::lang::jlong skip(::jxx::lang::jlong count) override;
    ::jxx::lang::jbool ready() override;
    ::jxx::lang::jbool markSupported() const override;
    void mark(::jxx::lang::jint readAheadLimit) override;
    void reset() override;
    void close() override;

private:
    void ensureOpen_() const;
    ::jxx::lang::jint fill_();

    ::jxx::Ptr<Reader> in_;
    ::jxx::lang::CharArray buffer_;
    ::jxx::lang::jint position_ = 0;
    ::jxx::lang::jint count_ = 0;
    ::jxx::lang::jint markedPosition_ = -1;
    ::jxx::lang::jint readAheadLimit_ = 0;
};

} // namespace jxx::io
