#pragma once
#include <cstdint>
#include <vector>
#include "io/jxx.io.Closeable.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::io { class File; class InputStream; }
namespace jxx::lang { class String; }
namespace jxx::util { template<typename E> class Enumeration; }
namespace jxx::util::zip { class ZipEntry;
class ZipFile final : public ::jxx::lang::ClassBase<ZipFile,::jxx::lang::Object,::jxx::io::Closeable> {
public:
 using JxxSuper=::jxx::lang::Object;
 explicit ZipFile(const ::jxx::Ptr<::jxx::lang::String>& name);
 explicit ZipFile(const ::jxx::Ptr<::jxx::io::File>& file);
 ~ZipFile()override;
 ::jxx::Ptr<ZipEntry> getEntry(const ::jxx::Ptr<::jxx::lang::String>& name)const;
 ::jxx::Ptr<::jxx::util::Enumeration<ZipEntry>> entries();
 ::jxx::Ptr<::jxx::io::InputStream> getInputStream(const ::jxx::Ptr<ZipEntry>& entry);
 ::jxx::Ptr<::jxx::lang::String> getName()const; ::jxx::lang::jint size()const noexcept; ::jxx::Ptr<::jxx::lang::String> getComment()const;
 void close()override;
private:
 void open_(const ::jxx::Ptr<::jxx::lang::String>& name); void ensureOpen_()const;
 std::uint16_t u16_(std::size_t offset)const; std::uint32_t u32_(std::size_t offset)const;
 ::jxx::Ptr<::jxx::lang::String> name_,comment_; std::vector<std::uint8_t> bytes_; std::vector<::jxx::Ptr<ZipEntry>> entries_; bool closed_=false;
}; }
