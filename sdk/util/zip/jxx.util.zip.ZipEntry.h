#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::lang { class String; }
namespace jxx::nio::file::attribute { class FileTime; }
namespace jxx::util::zip {
class ZipEntry final : public ::jxx::lang::ClassBase<ZipEntry,::jxx::lang::Object> {
public:
 static constexpr ::jxx::lang::jint STORED = 0;
 static constexpr ::jxx::lang::jint DEFLATED = 8;
 static constexpr ::jxx::lang::jint STORED_ = STORED;
 static constexpr ::jxx::lang::jint DEFLATED_ = DEFLATED;
 explicit ZipEntry(const ::jxx::Ptr<::jxx::lang::String>& name);
 ZipEntry(const ::jxx::Ptr<ZipEntry>& entry);
 ::jxx::Ptr<::jxx::lang::String> getName()const;
 ::jxx::lang::jbool isDirectory()const;
 ::jxx::lang::jlong getSize()const noexcept; void setSize(::jxx::lang::jlong value);
 ::jxx::lang::jlong getCompressedSize()const noexcept; void setCompressedSize(::jxx::lang::jlong value);
 ::jxx::lang::jlong getCrc()const noexcept; void setCrc(::jxx::lang::jlong value);
 ::jxx::lang::jint getMethod()const noexcept; void setMethod(::jxx::lang::jint value);
 ::jxx::lang::jlong getTime()const noexcept; void setTime(::jxx::lang::jlong value)noexcept;
 ::jxx::Ptr<::jxx::nio::file::attribute::FileTime> getLastModifiedTime()const;
 void setLastModifiedTime(const ::jxx::Ptr<::jxx::nio::file::attribute::FileTime>& value);
 ::jxx::Ptr<::jxx::nio::file::attribute::FileTime> getLastAccessTime()const;
 void setLastAccessTime(const ::jxx::Ptr<::jxx::nio::file::attribute::FileTime>& value);
 ::jxx::Ptr<::jxx::nio::file::attribute::FileTime> getCreationTime()const;
 void setCreationTime(const ::jxx::Ptr<::jxx::nio::file::attribute::FileTime>& value);
 ::jxx::Ptr<::jxx::lang::String> getComment()const; void setComment(const ::jxx::Ptr<::jxx::lang::String>& value);
 ::jxx::lang::ByteArray getExtra()const; void setExtra(const ::jxx::lang::ByteArray& value);
 ::jxx::Ptr<::jxx::lang::String> toString() const override;
 ::jxx::Ptr<::jxx::lang::Object> clone() const override;
 ::jxx::lang::jint hashCode()const override;
private:
 friend class ZipFile; friend class ZipOutputStream; friend class ZipInputStream;
 ::jxx::Ptr<::jxx::lang::String> name_,comment_; ::jxx::lang::ByteArray extra_;
 ::jxx::lang::jlong size_=-1,compressedSize_=-1,crc_=-1,time_=-1;
 ::jxx::Ptr<::jxx::nio::file::attribute::FileTime> lastModifiedTime_,lastAccessTime_,creationTime_;
 ::jxx::lang::jint method_=-1; ::jxx::lang::jlong localOffset_=0,dataOffset_=0;
};
}
