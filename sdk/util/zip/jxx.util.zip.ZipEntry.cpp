#include "util/zip/jxx.util.zip.ZipEntry.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
namespace jxx::util::zip {
ZipEntry::ZipEntry(const ::jxx::Ptr<::jxx::lang::String>&n):name_(n){if(n==nullptr)throw ::jxx::lang::NullPointerException();if(n->utf8().size()>65535)throw ::jxx::lang::IllegalArgumentException("entry name too long");}
ZipEntry::ZipEntry(const ::jxx::Ptr<ZipEntry>&e){if(e==nullptr)throw ::jxx::lang::NullPointerException();*this=*e;}
::jxx::Ptr<::jxx::lang::String> ZipEntry::getName()const{return name_;}
::jxx::lang::jbool ZipEntry::isDirectory()const{const auto n=name_->utf8();return !n.empty()&&n.back()=='/';}
::jxx::lang::jlong ZipEntry::getSize()const noexcept{return size_;} void ZipEntry::setSize(::jxx::lang::jlong v){if(v<0||v>0xffffffffLL)throw ::jxx::lang::IllegalArgumentException();size_=v;}
::jxx::lang::jlong ZipEntry::getCompressedSize()const noexcept{return compressedSize_;} void ZipEntry::setCompressedSize(::jxx::lang::jlong v){if(v<0)throw ::jxx::lang::IllegalArgumentException();compressedSize_=v;}
::jxx::lang::jlong ZipEntry::getCrc()const noexcept{return crc_;} void ZipEntry::setCrc(::jxx::lang::jlong v){if(v<0||v>0xffffffffLL)throw ::jxx::lang::IllegalArgumentException();crc_=v;}
::jxx::lang::jint ZipEntry::getMethod()const noexcept{return method_;} void ZipEntry::setMethod(::jxx::lang::jint v){if(v!=STORED&&v!=DEFLATED)throw ::jxx::lang::IllegalArgumentException();method_=v;}
::jxx::lang::jlong ZipEntry::getTime()const noexcept{return time_;} void ZipEntry::setTime(::jxx::lang::jlong v)noexcept{time_=v;}
::jxx::Ptr<::jxx::lang::String> ZipEntry::getComment()const{return comment_;} void ZipEntry::setComment(const ::jxx::Ptr<::jxx::lang::String>&v){comment_=v;}
::jxx::lang::ByteArray ZipEntry::getExtra()const{return extra_;} void ZipEntry::setExtra(const ::jxx::lang::ByteArray&v){if(v&&v->length>65535)throw ::jxx::lang::IllegalArgumentException();extra_=v;}
::jxx::Ptr<::jxx::lang::String> ZipEntry::toString() const{return name_;}
::jxx::Ptr<::jxx::lang::Object> ZipEntry::clone() const{auto copy=::jxx::NEW<ZipEntry>(name_);*copy=*this;return ::jxx::CAST<::jxx::lang::Object>(copy);}
::jxx::lang::jint ZipEntry::hashCode()const{return name_->hashCode();}
}
