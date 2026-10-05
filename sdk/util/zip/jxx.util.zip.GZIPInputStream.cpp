#include "util/zip/jxx.util.zip.GZIPInputStream.h"
#include <zlib.h>
#include "io/jxx.io.EOFException.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/zip/jxx.util.zip.ZipException.h"
namespace jxx::util::zip {namespace {constexpr ::jxx::lang::jint DEFAULT_SIZE=512;}
GZIPInputStream::GZIPInputStream(const ::jxx::Ptr<::jxx::io::InputStream>&i):GZIPInputStream(i,DEFAULT_SIZE){}
GZIPInputStream::GZIPInputStream(const ::jxx::Ptr<::jxx::io::InputStream>&i,::jxx::lang::jint s):Super(i,::jxx::NEW<Inflater>(true),s){crc_=static_cast<std::uint32_t>(::crc32(0L,Z_NULL,0));parseHeader_();}
GZIPInputStream::~GZIPInputStream(){if(inf)inf->end();}
void GZIPInputStream::ensureGzipOpen_()const{if(closed_)throw ::jxx::io::IOException("stream closed");}
::jxx::lang::jint GZIPInputStream::raw_(){if(pendingPos_<pending_.size())return pending_[pendingPos_++];return in_->read();}
std::uint32_t GZIPInputStream::u32_(){std::uint32_t v=0;for(int i=0;i<4;++i){auto b=raw_();if(b<0)throw ::jxx::io::EOFException("truncated GZIP trailer");v|=static_cast<std::uint32_t>(b)<<(8*i);}return v;}
void GZIPInputStream::parseHeader_(::jxx::lang::jint first){auto id1=first<0?raw_():first;auto id2=raw_();if(id1!=0x1f||id2!=0x8b)throw ZipException("not in GZIP format");auto cm=raw_();auto flg=raw_();if(cm!=8||(flg&0xe0)!=0)throw ZipException("invalid GZIP header");for(int i=0;i<6;++i)if(raw_()<0)throw ::jxx::io::EOFException("truncated GZIP header");if(flg&4){auto lo=raw_(),hi=raw_();if(lo<0||hi<0)throw ::jxx::io::EOFException("truncated GZIP extra field");auto n=lo|(hi<<8);while(n-->0)if(raw_()<0)throw ::jxx::io::EOFException("truncated GZIP extra field");}if(flg&8)while(true){auto b=raw_();if(b<0)throw ::jxx::io::EOFException("truncated GZIP name");if(b==0)break;}if(flg&16)while(true){auto b=raw_();if(b<0)throw ::jxx::io::EOFException("truncated GZIP comment");if(b==0)break;}if(flg&2){if(raw_()<0||raw_()<0)throw ::jxx::io::EOFException("truncated GZIP header CRC");}inf->reset();resetInflationState_();crc_=static_cast<std::uint32_t>(::crc32(0L,Z_NULL,0));size_=0;}
void GZIPInputStream::fill(){ensureGzipOpen_();::jxx::lang::jint count=0;while(pendingPos_<pending_.size()&&count<buf->length)(*buf)[count++]=static_cast<::jxx::lang::jbyte>(pending_[pendingPos_++]);if(pendingPos_>=pending_.size()){pending_.clear();pendingPos_=0;}if(count==0)count=in_->read(buf,0,buf->length);else if(count<buf->length){auto more=in_->read(buf,count,buf->length-count);if(more>0)count+=more;}if(count<=0)throw ::jxx::io::EOFException("Unexpected end of ZLIB input stream");len=count;inf->setInput(buf,0,len);}
void GZIPInputStream::preserveRemaining_(){auto r=inf->getRemaining();if(r>0){auto start=len-r;pending_.assign(static_cast<std::size_t>(r),0);for(::jxx::lang::jint i=0;i<r;++i)pending_[i]=static_cast<std::uint8_t>((*buf)[start+i]);pendingPos_=0;}}
bool GZIPInputStream::finishMember_(){preserveRemaining_();auto expectedCrc=u32_(),expectedSize=u32_();if(expectedCrc!=crc_||expectedSize!=size_)throw ZipException("corrupt GZIP trailer");auto first=raw_();if(first<0){eos_=true;return false;}parseHeader_(first);return true;}
::jxx::lang::jint GZIPInputStream::read(){auto a=::jxx::NEW<::jxx::lang::ByteArrayType>(1);auto n=read(a,0,1);return n<0?-1:static_cast<unsigned char>((*a)[0]);}
::jxx::lang::jint GZIPInputStream::read(const ::jxx::lang::ByteArray&b){if(!b)throw ::jxx::lang::NullPointerException();return read(b,0,b->length);}
::jxx::lang::jint GZIPInputStream::read(const ::jxx::lang::ByteArray&b,::jxx::lang::jint o,::jxx::lang::jint n){ensureGzipOpen_();if(!b)throw ::jxx::lang::NullPointerException();if(o<0||n<0||o>b->length-n)throw ::jxx::lang::ArrayIndexOutOfBoundsException();if(n==0)return 0;if(eos_)return -1;while(true){auto got=InflaterInputStream::read(b,o,n);if(got>0){crc_=static_cast<std::uint32_t>(::crc32(crc_,reinterpret_cast<const Bytef*>(&(*b)[o]),static_cast<uInt>(got)));size_+=static_cast<std::uint32_t>(got);return got;}if(!finishMember_())return -1;}}
::jxx::lang::jint GZIPInputStream::available(){ensureGzipOpen_();return eos_?0:1;}
void GZIPInputStream::close(){if(closed_)return;inf->end();in_->close();closed_=true;eos_=true;}
}
