#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::lang { class String; }
namespace jxx::util::zip {
class ZipEntry final : public ::jxx::lang::ClassBase<ZipEntry,::jxx::lang::Object> {
public:
 static constexpr ::jxx::lang::jint STORED_=0;
 static constexpr ::jxx::lang::jint DEFLATED_=8;
 explicit ZipEntry(const ::jxx::Ptr<::jxx::lang::String>& name);
 ZipEntry(const ::jxx::Ptr<ZipEntry>& entry);
 ::jxx::Ptr<::jxx::lang::String> getName()const;
 ::jxx::lang::jbool isDirectory()const;
 ::jxx::lang::jlong getSize()const noexcept; void setSize(::jxx::lang::jlong value);
 ::jxx::lang::jlong getCompressedSize()const noexcept; void setCompressedSize(::jxx::lang::jlong value);
 ::jxx::lang::jlong getCrc()const noexcept; void setCrc(::jxx::lang::jlong value);
 ::jxx::lang::jint getMethod()const noexcept; void setMethod(::jxx::lang::jint value);
 ::jxx::lang::jlong getTime()const noexcept; void setTime(::jxx::lang::jlong value)noexcept;
 ::jxx::Ptr<::jxx::lang::String> getComment()const; void setComment(const ::jxx::Ptr<::jxx::lang::String>& value);
 ::jxx::lang::ByteArray getExtra()const; void setExtra(const ::jxx::lang::ByteArray& value);
 ::jxx::lang::jint hashCode()const override;
private:
 friend class ZipFile; friend class ZipOutputStream;
 ::jxx::Ptr<::jxx::lang::String> name_,comment_; ::jxx::lang::ByteArray extra_;
 ::jxx::lang::jlong size_=-1,compressedSize_=-1,crc_=-1,time_=-1;
 ::jxx::lang::jint method_=DEFLATED_; ::jxx::lang::jlong localOffset_=0,dataOffset_=0;
};
}
