#pragma once
#include <string>
#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.Writer.h"
#include "lang/jxx.lang.String.h"
namespace com::google::gson::internal {
class StringWriter final : public ::jxx::lang::ClassBase<StringWriter, ::jxx::io::Writer> {
public:
 using JxxSuper = ::jxx::io::Writer;
 using Super = ::jxx::lang::ClassBase<StringWriter, JxxSuper>;
 void write(const ::jxx::lang::CharArray& b,::jxx::lang::jint o,::jxx::lang::jint n) override;
 void write(const ::jxx::Ptr<::jxx::lang::String>& value) override;
 void flush() override;
 void close() override;
 ::jxx::Ptr<::jxx::lang::String> toString() const;
private: std::string value_;
};}
