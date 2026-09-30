#pragma once
#include <cstdint>
#include <vector>
#include "io/jxx.io.FilterOutputStream.h"
namespace jxx::lang { class String; }
namespace jxx::util::zip { class ZipEntry;
class ZipOutputStream final : public ::jxx::lang::ClassBase<ZipOutputStream,::jxx::io::FilterOutputStream> {
public:
 using JxxSuper=::jxx::io::FilterOutputStream;
 using Super=::jxx::lang::ClassBase<ZipOutputStream,JxxSuper>;
 explicit ZipOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output);
 ~ZipOutputStream()override;
 void putNextEntry(const ::jxx::Ptr<ZipEntry>& entry);
 void closeEntry();
 void write(::jxx::lang::jint value)override;
 void write(const ::jxx::lang::ByteArray& buffer)override;
 void write(const ::jxx::lang::ByteArray& buffer,::jxx::lang::jint offset,::jxx::lang::jint length)override;
 void finish(); void close()override;
 void setMethod(::jxx::lang::jint method); void setLevel(::jxx::lang::jint level); void setComment(const ::jxx::Ptr<::jxx::lang::String>& comment);
private:
 struct Record { ::jxx::Ptr<ZipEntry> entry; std::vector<std::uint8_t> data; std::uint32_t crc=0; std::uint32_t offset=0; };
 void ensureOpen_()const; void emit_(std::uint8_t value); void emit16_(std::uint16_t value); void emit32_(std::uint32_t value); void emitBytes_(const std::string& value); void emitBytes_(const std::vector<std::uint8_t>& value);
 std::vector<Record> records_; ::jxx::Ptr<ZipEntry> current_; std::vector<std::uint8_t> currentData_; ::jxx::Ptr<::jxx::lang::String> comment_; std::uint32_t position_=0; ::jxx::lang::jint method_=0,level_=-1; bool finished_=false,closed_=false;
}; }
