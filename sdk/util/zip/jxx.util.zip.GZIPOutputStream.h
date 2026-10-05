#pragma once
#include <cstdint>
#include "util/zip/jxx.util.zip.DeflaterOutputStream.h"
namespace jxx::util::zip {
class GZIPOutputStream final : public ::jxx::lang::ClassBase<GZIPOutputStream,DeflaterOutputStream> {
public:
 using JxxSuper=DeflaterOutputStream;using Super=::jxx::lang::ClassBase<GZIPOutputStream,JxxSuper>;using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;
 explicit GZIPOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output);
 GZIPOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output,::jxx::lang::jint size);
 GZIPOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output,::jxx::lang::jbool syncFlush);
 GZIPOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output,::jxx::lang::jint size,::jxx::lang::jbool syncFlush);
 ~GZIPOutputStream() override;
 void write(::jxx::lang::jint value) override;
 void write(const ::jxx::lang::ByteArray& buffer) override;
 void write(const ::jxx::lang::ByteArray& buffer,::jxx::lang::jint offset,::jxx::lang::jint length) override;
 void finish();void close() override;
private:
 void writeHeader_();void writeTrailer_();void ensureGzipOpen_()const;
 std::uint32_t crc_=0;std::uint32_t size_=0;bool gzipFinished_=false;bool gzipClosed_=false;
};}
