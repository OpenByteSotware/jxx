#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "util/zip/jxx.util.zip.DeflaterOutputStream.h"
namespace jxx::lang { class String; }
namespace jxx::util::zip { class ZipEntry;
class ZipOutputStream final : public ::jxx::lang::ClassBase<ZipOutputStream, DeflaterOutputStream> {
public:
 using JxxSuper=DeflaterOutputStream; using Super=::jxx::lang::ClassBase<ZipOutputStream,JxxSuper>; using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;
 static constexpr ::jxx::lang::jint STORED=0; static constexpr ::jxx::lang::jint DEFLATED=8;
 explicit ZipOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output); ~ZipOutputStream() override;
 void putNextEntry(const ::jxx::Ptr<ZipEntry>& entry); void closeEntry();
 void write(::jxx::lang::jint value) override; void write(const ::jxx::lang::ByteArray& buffer) override; void write(const ::jxx::lang::ByteArray& buffer,::jxx::lang::jint offset,::jxx::lang::jint length) override;
 void finish(); void close() override; void setMethod(::jxx::lang::jint method); void setLevel(::jxx::lang::jint level); void setComment(const ::jxx::Ptr<::jxx::lang::String>& comment);
private:
 struct Record{::jxx::Ptr<ZipEntry> entry;std::uint32_t crc=0,compressedSize=0,size=0,offset=0;std::uint16_t flags=0;};
 void ensureZipOpen_()const; void emit_(std::uint8_t);void emit16_(std::uint16_t);void emit32_(std::uint32_t);void emitBytes_(const std::string&);void emitExtra_(const ::jxx::lang::ByteArray&);void resetRawDeflater_();
 std::vector<Record> records_;::jxx::Ptr<ZipEntry> current_;std::uint32_t currentCrc_=0,currentSize_=0,currentDataStart_=0,position_=0;::jxx::Ptr<::jxx::lang::String> comment_;::jxx::lang::jint method_=DEFLATED,level_=-1;bool zipFinished_=false,zipClosed_=false;
};}
