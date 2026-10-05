#include "util/zip/jxx.util.zip.ZipInputStream.h"
#include <algorithm>
#include <zlib.h>
#include "io/jxx.io.EOFException.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/zip/jxx.util.zip.ZipEntry.h"
#include "util/zip/jxx.util.zip.ZipException.h"
namespace jxx::util::zip {
ZipInputStream::ZipInputStream(const ::jxx::Ptr<::jxx::io::InputStream>& input):Super(input,::jxx::NEW<Inflater>(true),512){}
ZipInputStream::~ZipInputStream(){if(inf)inf->end();}
void ZipInputStream::fill(){if(closed_)throw ::jxx::io::IOException("stream closed");::jxx::lang::jint count=0;while(pendingPos_<pending_.size()&&count<buf->length)(*buf)[count++]=static_cast<::jxx::lang::jbyte>(pending_[pendingPos_++]);if(pendingPos_>=pending_.size()){pending_.clear();pendingPos_=0;}if(count==0)count=in_->read(buf,0,buf->length);else if(count<buf->length){auto more=in_->read(buf,count,buf->length-count);if(more>0)count+=more;}if(count<=0)throw ::jxx::io::EOFException("Unexpected end of ZLIB input stream");len=count;inf->setInput(buf,0,len);}
::jxx::lang::jint ZipInputStream::raw_(){if(pendingPos_<pending_.size())return pending_[pendingPos_++];return in_->read();}
void ZipInputStream::exact_(std::uint8_t* p,std::size_t count){for(std::size_t i=0;i<count;++i){const auto value=raw_();if(value<0)throw ::jxx::io::EOFException("truncated ZIP entry");p[i]=static_cast<std::uint8_t>(value);}}
std::uint16_t ZipInputStream::u16_(){std::uint8_t b[2];exact_(b,2);return static_cast<std::uint16_t>(b[0]|(b[1]<<8));}
std::uint32_t ZipInputStream::u32_(){std::uint8_t b[4];exact_(b,4);return static_cast<std::uint32_t>(b[0])|(static_cast<std::uint32_t>(b[1])<<8)|(static_cast<std::uint32_t>(b[2])<<16)|(static_cast<std::uint32_t>(b[3])<<24);}
::jxx::Ptr<ZipEntry> ZipInputStream::getNextEntry(){
 if(closed_)throw ::jxx::io::IOException("stream closed"); if(entry_)closeEntry();
 const auto first=raw_(); if(first<0)return nullptr; std::uint8_t rest[3]; exact_(rest,3);
 const auto signature=static_cast<std::uint32_t>(first)|(static_cast<std::uint32_t>(rest[0])<<8)|(static_cast<std::uint32_t>(rest[1])<<16)|(static_cast<std::uint32_t>(rest[2])<<24);
 if(signature==0x02014b50U||signature==0x06054b50U)return nullptr; if(signature!=0x04034b50U)throw ZipException("invalid local ZIP header");
 u16_();const auto flags=u16_();const auto method=u16_();u16_();u16_();const auto crc=u32_();const auto compressedSize=u32_();const auto size=u32_();const auto nameLength=u16_();const auto extraLength=u16_();
 if((flags&1U)!=0U)throw ZipException("encrypted ZIP entry"); if(method!=ZipEntry::STORED&&method!=ZipEntry::DEFLATED)throw ZipException("unsupported ZIP compression method");
 std::vector<std::uint8_t> name(nameLength);if(nameLength!=0)exact_(name.data(),nameLength);std::vector<std::uint8_t> extra(extraLength);if(extraLength!=0)exact_(extra.data(),extraLength);
 descriptor_=(flags&8U)!=0U; if(descriptor_&&method==ZipEntry::STORED)throw ZipException("STORED entry with data descriptor is not supported without a known size");
 entry_=::jxx::NEW<ZipEntry>(::jxx::NEW<::jxx::lang::String>(std::string(name.begin(),name.end())));entry_->method_=method;entry_->crc_=descriptor_?-1:crc;entry_->compressedSize_=descriptor_?-1:compressedSize;entry_->size_=descriptor_?-1:size;
 if(extraLength!=0){entry_->extra_=::jxx::NEW<::jxx::lang::ByteArrayType>(extraLength);for(::jxx::lang::jint i=0;i<extraLength;++i)(*entry_->extra_)[i]=static_cast<::jxx::lang::jbyte>(extra[static_cast<std::size_t>(i)]);}
 expectedCompressed_=compressedSize;crc_=static_cast<std::uint32_t>(::crc32(0L,Z_NULL,0));readCount_=0;entryEof_=false;if(method==ZipEntry::DEFLATED){inf->reset();resetInflationState_();}return entry_;
}
void ZipInputStream::preserveRemaining_(){const auto remaining=inf->getRemaining();if(remaining>0){const auto start=len-remaining;pending_.assign(static_cast<std::size_t>(remaining),0);for(::jxx::lang::jint i=0;i<remaining;++i)pending_[static_cast<std::size_t>(i)]=static_cast<std::uint8_t>((*buf)[start+i]);pendingPos_=0;}}
void ZipInputStream::verify_(){
 std::uint32_t compressed=entry_->method_==ZipEntry::DEFLATED?static_cast<std::uint32_t>(inf->getBytesRead()):readCount_;
 if(descriptor_){const auto first=u32_();const auto crc=first==0x08074b50U?u32_():first;const auto compressedSize=u32_();const auto size=u32_();entry_->crc_=crc;entry_->compressedSize_=compressedSize;entry_->size_=size;}
 if(static_cast<std::uint32_t>(entry_->crc_)!=crc_||static_cast<std::uint32_t>(entry_->size_)!=readCount_)throw ZipException("invalid ZIP entry CRC or size");
 if(static_cast<std::uint32_t>(entry_->compressedSize_)!=compressed)throw ZipException("invalid ZIP entry compressed size");
}
::jxx::lang::jint ZipInputStream::read(){auto one=::jxx::NEW<::jxx::lang::ByteArrayType>(1);const auto count=read(one,0,1);return count<0?-1:static_cast<unsigned char>((*one)[0]);}
::jxx::lang::jint ZipInputStream::read(const ::jxx::lang::ByteArray& b){if(!b)throw ::jxx::lang::NullPointerException();return read(b,0,b->length);}
::jxx::lang::jint ZipInputStream::read(const ::jxx::lang::ByteArray& b,::jxx::lang::jint offset,::jxx::lang::jint count){if(closed_)throw ::jxx::io::IOException("stream closed");if(!b)throw ::jxx::lang::NullPointerException();if(offset<0||count<0||offset>b->length-count)throw ::jxx::lang::ArrayIndexOutOfBoundsException();if(count==0)return 0;if(!entry_||entryEof_)return -1;::jxx::lang::jint result=0;if(entry_->method_==ZipEntry::STORED){const auto remaining=entry_->size_-readCount_;if(remaining<=0){entryEof_=true;verify_();return -1;}result=static_cast<::jxx::lang::jint>(std::min<::jxx::lang::jlong>(remaining,count));for(::jxx::lang::jint i=0;i<result;++i){const auto value=raw_();if(value<0)throw ::jxx::io::EOFException("truncated ZIP entry");(*b)[offset+i]=static_cast<::jxx::lang::jbyte>(value);}}else{result=InflaterInputStream::read(b,offset,count);if(result<0){preserveRemaining_();entryEof_=true;verify_();return -1;}}crc_=static_cast<std::uint32_t>(::crc32(crc_,reinterpret_cast<const Bytef*>(&(*b)[offset]),static_cast<uInt>(result)));readCount_+=static_cast<std::uint32_t>(result);return result;}
void ZipInputStream::closeEntry(){if(!entry_)return;auto temp=::jxx::NEW<::jxx::lang::ByteArrayType>(512);while(read(temp)!=-1){}entry_.reset();entryEof_=true;}
::jxx::lang::jlong ZipInputStream::skip(::jxx::lang::jlong count){if(count<0)throw ::jxx::lang::IllegalArgumentException();auto temp=::jxx::NEW<::jxx::lang::ByteArrayType>(512);::jxx::lang::jlong total=0;while(total<count){const auto value=read(temp,0,static_cast<::jxx::lang::jint>(std::min<::jxx::lang::jlong>(count-total,temp->length)));if(value<0)break;total+=value;}return total;}
::jxx::lang::jint ZipInputStream::available(){if(closed_)throw ::jxx::io::IOException("stream closed");return entry_&&!entryEof_?1:0;}
void ZipInputStream::close(){if(closed_)return;inf->end();in_->close();closed_=true;entry_.reset();entryEof_=true;}
}
