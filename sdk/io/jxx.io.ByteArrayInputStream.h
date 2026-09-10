#pragma once
#include "io/jxx.io.InputStream.h"
namespace jxx::io {
class ByteArrayInputStream final : public ::jxx::lang::ClassBase<ByteArrayInputStream,InputStream> {
public:
 using JxxSuper=InputStream; using Super=::jxx::lang::ClassBase<ByteArrayInputStream,JxxSuper>;
 explicit ByteArrayInputStream(const ::jxx::lang::ByteArray& buffer); ByteArrayInputStream(const ::jxx::lang::ByteArray& buffer,::jxx::lang::jint offset,::jxx::lang::jint length);
 ::jxx::lang::jint read() override; ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer,::jxx::lang::jint offset,::jxx::lang::jint length) override; ::jxx::lang::jlong skip(::jxx::lang::jlong count) override; ::jxx::lang::jint available() override; void close() override; void mark(::jxx::lang::jint readLimit) override; void reset() override; ::jxx::lang::jbool markSupported() const override;
private: ::jxx::lang::ByteArray buffer_; ::jxx::lang::jint position_=0,mark_=0,count_=0;
};
} // namespace jxx::io
