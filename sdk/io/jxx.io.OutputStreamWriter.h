#pragma once
#include <string>
#include "io/jxx.io.Writer.h"
namespace jxx::io { class OutputStream;
class OutputStreamWriter : public ::jxx::lang::ClassBase<OutputStreamWriter,Writer> {
public:
 using JxxSuper=Writer; using Super=::jxx::lang::ClassBase<OutputStreamWriter,JxxSuper>;
 explicit OutputStreamWriter(const ::jxx::Ptr<OutputStream>& output); OutputStreamWriter(const ::jxx::Ptr<OutputStream>& output,const ::jxx::Ptr<::jxx::lang::String>& charsetName); void write(::jxx::lang::jint value) override; void write(const ::jxx::lang::CharArray& buffer,::jxx::lang::jint offset,::jxx::lang::jint length) override; void write(const ::jxx::Ptr<::jxx::lang::String>& value,::jxx::lang::jint offset,::jxx::lang::jint length) override; void flush() override; void close() override; ::jxx::Ptr<::jxx::lang::String> getEncoding()const;
private: void writeString(const ::jxx::Ptr<::jxx::lang::String>& value); ::jxx::Ptr<OutputStream> out_; ::jxx::Ptr<::jxx::lang::String> encoding_;
}; }
