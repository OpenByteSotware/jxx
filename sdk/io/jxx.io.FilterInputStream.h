#pragma once
#include "io/jxx.io.InputStream.h"
namespace jxx::io {
class FilterInputStream : public ::jxx::lang::ClassBase<FilterInputStream, InputStream> {
public:
    using JxxSuper = InputStream; using Super = ::jxx::lang::ClassBase<FilterInputStream,JxxSuper>;
    explicit FilterInputStream(const ::jxx::Ptr<InputStream>& input);
    ~FilterInputStream() override = default;
    ::jxx::lang::jint read() override;
    ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer) override;
    ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer,::jxx::lang::jint offset,::jxx::lang::jint length) override;
    ::jxx::lang::jlong skip(::jxx::lang::jlong count) override;
    ::jxx::lang::jint available() override;
    void close() override; void mark(::jxx::lang::jint readLimit) override; void reset() override; ::jxx::lang::jbool markSupported() const override;
protected: ::jxx::Ptr<InputStream> in_;
};
} // namespace jxx::io
