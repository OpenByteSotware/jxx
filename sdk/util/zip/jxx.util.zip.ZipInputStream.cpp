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
ZipInputStream::ZipInputStream(const ::jxx::Ptr<::jxx::io::InputStream>&i):Super(i,::jxx::NEW<Inflater>(true),512){}
ZipInputStream::~ZipInputStream(){if(inf)inf->end();}
void ZipInputStream::fill(){
    if(closed_)throw ::jxx::io::IOException("stream closed");
    ::jxx::lang::jint count=0;
    while(pendingPos_<pending_.size()&&count<buf->length){
        (*buf)[count++]=static_cast<::jxx::lang::jbyte>(pending_[pendingPos_++]);
    }
    if(pendingPos_>=pending_.size()){
        pending_.clear();
        pendingPos_=0;
    }
    if(count==0){
        count=in_->read(buf,0,buf->length);
    }else if(count<buf->length){
        const auto additional=in_->read(buf,count,buf->length-count);
        if(additional>0)count+=additional;
    }
    if(count<=0)throw ::jxx::io::EOFException("Unexpected end of ZLIB input stream");
    len=count;
    inf->setInput(buf,0,len);
}

::jxx::lang::jint ZipInputStream::raw_(){if(pendingPos_<pending_.size())return pending_[pendingPos_++];return in_->read();}
void ZipInputStream::exact_(std::uint8_t*p,std::size_t n){for(std::size_t i=0;i<n;++i){auto v=raw_();if(v<0)throw ::jxx::io::EOFException("truncated ZIP entry");p[i]=static_cast<std::uint8_t>(v);}}
std::uint16_t ZipInputStream::u16_(){std::uint8_t b[2];exact_(b,2);return b[0]|(b[1]<<8);}std::uint32_t ZipInputStream::u32_(){std::uint8_t b[4];exact_(b,4);return b[0]|(b[1]<<8)|(b[2]<<16)|(static_cast<std::uint32_t>(b[3])<<24);}
::jxx::Ptr<ZipEntry> ZipInputStream::getNextEntry(){if(closed_)throw ::jxx::io::IOException("stream closed");if(entry_)closeEntry();auto sig=u32_();if(sig==0x02014b50U||sig==0x06054b50U)return nullptr;if(sig!=0x04034b50U)throw ZipException("invalid local ZIP header");u16_();auto flags=u16_();auto method=u16_();u16_();u16_();auto crc=u32_();auto csize=u32_();auto size=u32_();auto nl=u16_();auto xl=u16_();if(flags&1U)throw ZipException("encrypted ZIP entry");std::vector<std::uint8_t> name(nl);if(nl)exact_(name.data(),nl);std::vector<std::uint8_t> extra(xl);if(xl)exact_(extra.data(),xl);entry_=::jxx::NEW<ZipEntry>(::jxx::NEW<::jxx::lang::String>(std::string(name.begin(),name.end())));descriptor_=(flags&8U)!=0;entry_->method_=method;entry_->crc_=descriptor_?-1:crc;entry_->compressedSize_=descriptor_?-1:csize;entry_->size_=descriptor_?-1:size;if(xl){entry_->extra_=::jxx::NEW<::jxx::lang::ByteArrayType>(xl);for(::jxx::lang::jint i=0;i<xl;++i)(*entry_->extra_)[i]=static_cast<::jxx::lang::jbyte>(extra[i]);}expectedCompressed_=csize;crc_=::crc32(0L,Z_NULL,0);readCount_=0;entryEof_=false;if(method==ZipEntry::DEFLATED){inf->reset();resetInflationState_();}else if(method!=ZipEntry::STORED)throw ZipException("unsupported ZIP compression method");return entry_;}
void ZipInputStream::preserveRemaining_(){auto r=inf->getRemaining();if(r>0){auto start=len-r;pending_.assign(static_cast<std::size_t>(r),0);for(::jxx::lang::jint i=0;i<r;++i)pending_[i]=static_cast<std::uint8_t>((*buf)[start+i]);pendingPos_=0;}}
void ZipInputStream::verify_(){if(descriptor_){auto first=u32_();std::uint32_t crc=first==0x08074b50U?u32_():first;auto cs=u32_();auto sz=u32_();entry_->crc_=crc;entry_->compressedSize_=cs;entry_->size_=sz;}if(static_cast<std::uint32_t>(entry_->crc_)!=crc_||static_cast<std::uint32_t>(entry_->size_)!=readCount_)throw ZipException("invalid ZIP entry CRC or size");}
::jxx::lang::jint ZipInputStream::read(){auto a=::jxx::NEW<::jxx::lang::ByteArrayType>(1);auto n=read(a,0,1);return n<0?-1:static_cast<unsigned char>((*a)[0]);}
::jxx::lang::jint ZipInputStream::read(const ::jxx::lang::ByteArray&b){if(!b)throw ::jxx::lang::NullPointerException();return read(b,0,b->length);}
::jxx::lang::jint ZipInputStream::read(const ::jxx::lang::ByteArray&b,::jxx::lang::jint o,::jxx::lang::jint n){if(closed_)throw ::jxx::io::IOException("stream closed");if(!b)throw ::jxx::lang::NullPointerException();if(o<0||n<0||o>b->length-n)throw ::jxx::lang::ArrayIndexOutOfBoundsException();if(n==0)return 0;if(!entry_||entryEof_)return -1;::jxx::lang::jint got=0;if(entry_->method_==ZipEntry::STORED){auto remain=entry_->size_-readCount_;if(remain<=0){entryEof_=true;verify_();return -1;}got=static_cast<::jxx::lang::jint>(std::min<::jxx::lang::jlong>(remain,n));for(::jxx::lang::jint i=0;i<got;++i){auto v=raw_();if(v<0)throw ::jxx::io::EOFException("truncated ZIP entry");(*b)[o+i]=static_cast<::jxx::lang::jbyte>(v);}}else{try{got=InflaterInputStream::read(b,o,n);}catch(...){throw;}if(got<0){preserveRemaining_();entryEof_=true;verify_();return -1;}}crc_=static_cast<std::uint32_t>(::crc32(crc_,reinterpret_cast<const Bytef*>(&(*b)[o]),static_cast<uInt>(got)));readCount_+=got;return got;}
void ZipInputStream::closeEntry(){if(!entry_)return;auto temp=::jxx::NEW<::jxx::lang::ByteArrayType>(512);while(read(temp)!=-1){}entry_.reset();entryEof_=true;}
::jxx::lang::jlong ZipInputStream::skip(::jxx::lang::jlong n){if(n<0)throw ::jxx::lang::IllegalArgumentException();auto a=::jxx::NEW<::jxx::lang::ByteArrayType>(512);::jxx::lang::jlong t=0;while(t<n){auto r=read(a,0,static_cast<::jxx::lang::jint>(std::min<::jxx::lang::jlong>(n-t,a->length)));if(r<0)break;t+=r;}return t;}
::jxx::lang::jint ZipInputStream::available(){if(closed_)throw ::jxx::io::IOException("stream closed");return entry_&&!entryEof_?1:0;}
void ZipInputStream::close(){if(closed_)return;inf->end();in_->close();closed_=true;}
}
