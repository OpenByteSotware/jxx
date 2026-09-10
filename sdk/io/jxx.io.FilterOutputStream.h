#pragma once
#include "io/jxx.io.OutputStream.h"
namespace jxx::io {
class FilterOutputStream : public ::jxx::lang::ClassBase<FilterOutputStream, OutputStream> {
public:
    using JxxSuper=OutputStream; using Super=::jxx::lang::ClassBase<FilterOutputStream,JxxSuper>;
    explicit FilterOutputStream(const ::jxx::Ptr<OutputStream>& output); ~FilterOutputStream() override=default;
    void write(::jxx::lang::jint value) override; void write(const ::jxx::lang::ByteArray& buffer) override; void write(const ::jxx::lang::ByteArray& buffer,::jxx::lang::jint offset,::jxx::lang::jint length) override; void flush() override; void close() override;
protected: ::jxx::Ptr<OutputStream> out_;
};
} // namespace jxx::io
