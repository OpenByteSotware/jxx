#pragma once
#include <vector>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "nio/jxx.nio.ByteBuffer.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.OutputStream.h"
namespace jxx::util {
class Base64 : public ::jxx::lang::ClassBase<Base64, ::jxx::lang::Object> {
public:
 class Encoder; class Decoder;
 static ::jxx::Ptr<Encoder> getEncoder(); static ::jxx::Ptr<Encoder> getUrlEncoder(); static ::jxx::Ptr<Encoder> getMimeEncoder();
 static ::jxx::Ptr<Encoder> getMimeEncoder(::jxx::lang::jint lineLength,const ::jxx::lang::ByteArray& lineSeparator);
 static ::jxx::Ptr<Decoder> getDecoder(); static ::jxx::Ptr<Decoder> getUrlDecoder(); static ::jxx::Ptr<Decoder> getMimeDecoder();
};
class Base64::Encoder : public ::jxx::lang::ClassBase<Encoder, ::jxx::lang::Object> {
public:
 Encoder(::jxx::lang::jbool url,::jxx::lang::jbool padding,::jxx::lang::jint lineLength,const ::jxx::lang::ByteArray& separator);
 ::jxx::lang::ByteArray encode(const ::jxx::lang::ByteArray& src) const; ::jxx::lang::jint encode(const ::jxx::lang::ByteArray& src,const ::jxx::lang::ByteArray& dst) const;
 ::jxx::Ptr<::jxx::nio::ByteBuffer> encode(const ::jxx::Ptr<::jxx::nio::ByteBuffer>& src) const; ::jxx::Ptr<::jxx::lang::String> encodeToString(const ::jxx::lang::ByteArray& src) const;
 ::jxx::Ptr<Encoder> withoutPadding() const; ::jxx::Ptr<::jxx::io::OutputStream> wrap(const ::jxx::Ptr<::jxx::io::OutputStream>& stream) const;
private: ::jxx::lang::jbool url_,padding_; ::jxx::lang::jint lineLength_; ::jxx::lang::ByteArray separator_;
};
class Base64::Decoder : public ::jxx::lang::ClassBase<Decoder, ::jxx::lang::Object> {
public:
 Decoder(::jxx::lang::jbool url,::jxx::lang::jbool mime); ::jxx::lang::ByteArray decode(const ::jxx::lang::ByteArray& src) const; ::jxx::lang::jint decode(const ::jxx::lang::ByteArray& src,const ::jxx::lang::ByteArray& dst) const;
 ::jxx::lang::ByteArray decode(const ::jxx::Ptr<::jxx::lang::String>& src) const; ::jxx::Ptr<::jxx::nio::ByteBuffer> decode(const ::jxx::Ptr<::jxx::nio::ByteBuffer>& src) const; ::jxx::Ptr<::jxx::io::InputStream> wrap(const ::jxx::Ptr<::jxx::io::InputStream>& stream) const;
private: ::jxx::lang::jbool url_,mime_;
};
class Base64EncodingOutputStream final : public ::jxx::io::OutputStream { public: Base64EncodingOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& out,const ::jxx::Ptr<Base64::Encoder>& encoder); void write(::jxx::lang::jint value) override; void write(const ::jxx::lang::ByteArray& b) override; void write(const ::jxx::lang::ByteArray& b,::jxx::lang::jint o,::jxx::lang::jint n) override; void flush() override; void close() override; private: ::jxx::Ptr<::jxx::io::OutputStream> out_; ::jxx::Ptr<Base64::Encoder> encoder_; std::vector<::jxx::lang::jbyte> pending_; bool closed_=false; };
class Base64DecodingInputStream final : public ::jxx::io::InputStream { public: Base64DecodingInputStream(const ::jxx::Ptr<::jxx::io::InputStream>& in,const ::jxx::Ptr<Base64::Decoder>& decoder); ::jxx::lang::jint read() override; ::jxx::lang::jint read(const ::jxx::lang::ByteArray& b,::jxx::lang::jint o,::jxx::lang::jint n) override; ::jxx::lang::jint available() override; void close() override; private: void load_(); ::jxx::Ptr<::jxx::io::InputStream> in_; ::jxx::Ptr<Base64::Decoder> decoder_; ::jxx::lang::ByteArray decoded_; ::jxx::lang::jint position_=0; bool loaded_=false; };
} // namespace jxx::util
