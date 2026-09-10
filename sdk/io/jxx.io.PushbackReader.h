#pragma once

#include "io/jxx.io.FilterReader.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace jxx::io {

class PushbackReader
    : public ::jxx::lang::ClassBase<PushbackReader, FilterReader> {
public:
    using JxxSuper = FilterReader;
    using Super = ::jxx::lang::ClassBase<PushbackReader, JxxSuper>;
    using JxxSuper::read;

    explicit PushbackReader(const ::jxx::Ptr<Reader>& input);
    PushbackReader(const ::jxx::Ptr<Reader>& input, ::jxx::lang::jint size);
    ~PushbackReader() override;

    ::jxx::lang::jint read() override;
    ::jxx::lang::jint read(const ::jxx::lang::CharArray& buffer,
                           ::jxx::lang::jint offset,
                           ::jxx::lang::jint length) override;
    void unread(::jxx::lang::jint value);
    void unread(const ::jxx::lang::CharArray& buffer);
    void unread(const ::jxx::lang::CharArray& buffer,
                ::jxx::lang::jint offset,
                ::jxx::lang::jint length);
    ::jxx::lang::jbool ready() override;
    ::jxx::lang::jlong skip(::jxx::lang::jlong count) override;
    ::jxx::lang::jbool markSupported() const override;
    void mark(::jxx::lang::jint readAheadLimit) override;
    void reset() override;
    void close() override;

private:
    static ::jxx::lang::jint validateSize_(::jxx::lang::jint size);
    void ensureOpen_() const;
    ::jxx::lang::CharArray buffer_;
    ::jxx::lang::jint position_ = 0;
    ::jxx::lang::jbool closed_ = false;
};

} // namespace jxx::io
