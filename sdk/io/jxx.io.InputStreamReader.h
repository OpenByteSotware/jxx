#pragma once
#include <string>
#include "io/jxx.io.Reader.h"
namespace jxx::io { class InputStream;
class InputStreamReader : public ::jxx::lang::ClassBase<InputStreamReader,Reader> {
public:
 using JxxSuper=Reader; using Super=::jxx::lang::ClassBase<InputStreamReader,JxxSuper>;
 explicit InputStreamReader(const ::jxx::Ptr<InputStream>& input); InputStreamReader(const ::jxx::Ptr<InputStream>& input,const ::jxx::Ptr<::jxx::lang::String>& charsetName); ::jxx::lang::jint read() override; ::jxx::lang::jint read(const ::jxx::lang::CharArray& buffer,::jxx::lang::jint offset,::jxx::lang::jint length) override; ::jxx::lang::jbool ready() override; void close() override; ::jxx::Ptr<::jxx::lang::String> getEncoding() const;
private: void decodeAll(); ::jxx::Ptr<InputStream> in_; ::jxx::Ptr<::jxx::lang::String> encoding_; ::jxx::Ptr<::jxx::lang::String> decoded_; ::jxx::lang::jint position_=0;
}; }
