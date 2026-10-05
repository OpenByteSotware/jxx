#include "util/zip/jxx.util.zip.ZipOutputStream.h"
#include <chrono>
#include <ctime>
#include <limits>
#include <unordered_set>
#include <zlib.h>
#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/zip/jxx.util.zip.ZipEntry.h"
#include "util/zip/jxx.util.zip.ZipException.h"
namespace jxx::util::zip { namespace {
std::pair<std::uint16_t,std::uint16_t> dosTime(::jxx::lang::jlong ms){if(ms<0)ms=static_cast<::jxx::lang::jlong>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());std::time_t t=static_cast<std::time_t>(ms/1000);std::tm l{};
#ifdef _WIN32
 localtime_s(&l,&t);
#else
 localtime_r(&t,&l);
#endif
 int y=l.tm_year+1900;if(y<1980)y=1980;return {static_cast<std::uint16_t>(((y-1980)<<9)|((l.tm_mon+1)<<5)|l.tm_mday),static_cast<std::uint16_t>((l.tm_hour<<11)|(l.tm_min<<5)|(l.tm_sec/2))};}
void require32(std::uint64_t v){if(v>0xffffffffULL)throw ZipException("ZIP64 is not supported");}
}
ZipOutputStream::ZipOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& o):Super(o,::jxx::NEW<Deflater>(Deflater::DEFAULT_COMPRESSION,true),512,false){}
ZipOutputStream::~ZipOutputStream(){if(def)def->end();}
void ZipOutputStream::ensureZipOpen_()const{if(zipClosed_)throw ZipException("stream closed");}
void ZipOutputStream::emit_(std::uint8_t v){out_->write(static_cast<::jxx::lang::jint>(v));++position_;}
void ZipOutputStream::emit16_(std::uint16_t v){emit_(v);emit_(v>>8);}void ZipOutputStream::emit32_(std::uint32_t v){emit16_(v);emit16_(v>>16);}void ZipOutputStream::emitBytes_(const std::string&s){for(unsigned char c:s)emit_(c);}void ZipOutputStream::emitExtra_(const ::jxx::lang::ByteArray&e){if(e)for(::jxx::lang::jint i=0;i<e->length;++i)emit_(static_cast<std::uint8_t>((*e)[i]));}
void ZipOutputStream::resetRawDeflater_(){def->end();def=::jxx::NEW<Deflater>(level_,true);}
void ZipOutputStream::putNextEntry(const ::jxx::Ptr<ZipEntry>& e){ensureZipOpen_();if(zipFinished_)throw ZipException("stream finished");if(!e)throw ::jxx::lang::NullPointerException();if(current_)closeEntry();for(auto&r:records_)if(r.entry->getName()->equals(e->getName()))throw ZipException("duplicate entry");auto name=e->getName()->utf8();if(name.size()>65535)throw ::jxx::lang::IllegalArgumentException();auto extra=e->getExtra();if(extra&&extra->length>65535)throw ::jxx::lang::IllegalArgumentException();if(e->method_<0)e->method_=method_;if(e->method_!=STORED&&e->method_!=DEFLATED)throw ZipException("unsupported compression method");if(e->method_==STORED&&(e->size_<0||e->crc_<0))throw ZipException("STORED entry requires size and CRC");Record r;r.entry=e;r.offset=position_;r.flags=0x0800U;if(e->method_==DEFLATED)r.flags|=0x0008U;records_.push_back(r);current_=e;currentCrc_=::crc32(0L,Z_NULL,0);currentSize_=0;auto dt=dosTime(e->time_);emit32_(0x04034b50U);emit16_(20);emit16_(r.flags);emit16_(static_cast<std::uint16_t>(e->method_));emit16_(dt.second);emit16_(dt.first);emit32_(e->method_==STORED?static_cast<std::uint32_t>(e->crc_):0);emit32_(e->method_==STORED?static_cast<std::uint32_t>(e->size_):0);emit32_(e->method_==STORED?static_cast<std::uint32_t>(e->size_):0);emit16_(static_cast<std::uint16_t>(name.size()));emit16_(extra?static_cast<std::uint16_t>(extra->length):0);emitBytes_(name);emitExtra_(extra);currentDataStart_=position_;if(e->method_==DEFLATED)resetRawDeflater_();}
void ZipOutputStream::write(::jxx::lang::jint v){auto a=::jxx::NEW<::jxx::lang::ByteArrayType>(1);(*a)[0]=static_cast<::jxx::lang::jbyte>(v);write(a,0,1);}void ZipOutputStream::write(const ::jxx::lang::ByteArray&b){if(!b)throw ::jxx::lang::NullPointerException();write(b,0,b->length);}void ZipOutputStream::write(const ::jxx::lang::ByteArray&b,::jxx::lang::jint o,::jxx::lang::jint n){ensureZipOpen_();if(!b)throw ::jxx::lang::NullPointerException();if(o<0||n<0||o>b->length-n)throw ::jxx::lang::ArrayIndexOutOfBoundsException();if(!current_)throw ZipException("no current ZIP entry");currentCrc_=static_cast<std::uint32_t>(::crc32(currentCrc_,reinterpret_cast<const Bytef*>(&(*b)[o]),static_cast<uInt>(n)));currentSize_+=static_cast<std::uint32_t>(n);if(current_->method_==STORED)out_->write(b,o,n);else{def->setInput(b,o,n);while(!def->needsInput()){auto c=def->deflate(buf,0,buf->length);if(c>0)out_->write(buf,0,c),position_+=static_cast<std::uint32_t>(c);}}if(current_->method_==STORED)position_+=static_cast<std::uint32_t>(n);}
void ZipOutputStream::closeEntry(){ensureZipOpen_();if(!current_)throw ZipException("no current ZIP entry");auto&r=records_.back();if(current_->method_==DEFLATED){def->finish();while(!def->finished()){auto c=def->deflate(buf,0,buf->length);if(c>0)out_->write(buf,0,c),position_+=static_cast<std::uint32_t>(c);}r.compressedSize=position_-currentDataStart_;emit32_(0x08074b50U);emit32_(currentCrc_);emit32_(r.compressedSize);emit32_(currentSize_);}else{r.compressedSize=currentSize_;if(current_->size_!=currentSize_||static_cast<std::uint32_t>(current_->crc_)!=currentCrc_)throw ZipException("invalid STORED entry size or CRC");}r.size=currentSize_;r.crc=currentCrc_;current_->size_=r.size;current_->compressedSize_=r.compressedSize;current_->crc_=r.crc;current_.reset();}
void ZipOutputStream::finish(){ensureZipOpen_();if(zipFinished_)return;if(current_)closeEntry();require32(records_.size());auto central=position_;for(auto&r:records_){auto name=r.entry->getName()->utf8();auto extra=r.entry->getExtra();auto comment=r.entry->comment_?r.entry->comment_->utf8():std::string();auto dt=dosTime(r.entry->time_);emit32_(0x02014b50U);emit16_(20);emit16_(20);emit16_(r.flags);emit16_(r.entry->method_);emit16_(dt.second);emit16_(dt.first);emit32_(r.crc);emit32_(r.compressedSize);emit32_(r.size);emit16_(name.size());emit16_(extra?extra->length:0);emit16_(comment.size());emit16_(0);emit16_(0);emit32_(r.entry->isDirectory()?0x10U:0U);emit32_(r.offset);emitBytes_(name);emitExtra_(extra);emitBytes_(comment);}auto centralSize=position_-central;auto c=comment_?comment_->utf8():std::string();emit32_(0x06054b50U);emit16_(0);emit16_(0);emit16_(records_.size());emit16_(records_.size());emit32_(centralSize);emit32_(central);emit16_(c.size());emitBytes_(c);out_->flush();zipFinished_=true;}
void ZipOutputStream::close(){if(zipClosed_)return;try{finish();out_->close();}catch(...){zipClosed_=true;def->end();throw;}zipClosed_=true;def->end();}
void ZipOutputStream::setMethod(::jxx::lang::jint m){ensureZipOpen_();if(m!=STORED&&m!=DEFLATED)throw ::jxx::lang::IllegalArgumentException();method_=m;}void ZipOutputStream::setLevel(::jxx::lang::jint l){ensureZipOpen_();if(l<-1||l>9)throw ::jxx::lang::IllegalArgumentException();level_=l;}void ZipOutputStream::setComment(const ::jxx::Ptr<::jxx::lang::String>&c){ensureZipOpen_();if(c&&c->utf8().size()>65535)throw ::jxx::lang::IllegalArgumentException();comment_=c;}
}
