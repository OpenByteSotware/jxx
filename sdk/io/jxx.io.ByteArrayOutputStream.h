#pragma once
#include "io/jxx.io.OutputStream.h"
namespace jxx::lang { class String; }
namespace jxx::io {
class ByteArrayOutputStream final : public ::jxx::lang::ClassBase<ByteArrayOutputStream,OutputStream> {
public:
 using JxxSuper=OutputStream; using Super=::jxx::lang::ClassBase<ByteArrayOutputStream,JxxSuper>;
 ByteArrayOutputStream(); explicit ByteArrayOutputStream(::jxx::lang::jint size); void write(::jxx::lang::jint value) override; void write(const ::jxx::lang::ByteArray& buffer,::jxx::lang::jint offset,::jxx::lang::jint length) override; void writeTo(const ::jxx::Ptr<OutputStream>& output) const; void reset(); ::jxx::lang::ByteArray toByteArray() const; ::jxx::lang::jint size() const; ::jxx::Ptr<::jxx::lang::String> toString() const override; void close() override;
private: void ensureCapacity(::jxx::lang::jint capacity); ::jxx::lang::ByteArray buffer_; ::jxx::lang::jint count_=0;
};
} // namespace jxx::io
