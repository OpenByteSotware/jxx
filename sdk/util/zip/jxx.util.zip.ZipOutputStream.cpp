#include "util/zip/jxx.util.zip.ZipOutputStream.h"
#include <algorithm>
#include <unordered_set>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/zip/jxx.util.zip.ZipEntry.h"
#include "util/zip/jxx.util.zip.ZipException.h"
namespace { std::uint32_t crc32_(const std::vector<std::uint8_t>&d){std::uint32_t c=0xffffffffU;for(auto b:d){c^=b;for(int k=0;k<8;++k)c=(c>>1)^(0xedb88320U&static_cast<std::uint32_t>(-static_cast<int>(c&1)));}return c^0xffffffffU;} }
namespace jxx::util::zip {
ZipOutputStream::ZipOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>&o):Super(o){if(o==nullptr)throw ::jxx::lang::NullPointerException();}
ZipOutputStream::~ZipOutputStream(){try{close();}catch(...){}}
void ZipOutputStream::ensureOpen_()const{if(closed_)throw ZipException("stream closed");}
void ZipOutputStream::putNextEntry(const ::jxx::Ptr<ZipEntry>&e){ensureOpen_();if(e==nullptr)throw ::jxx::lang::NullPointerException();if(current_!=nullptr)closeEntry();for(const auto&r:records_)if(r.entry->getName()->equals(e->getName()))throw ZipException("duplicate entry");current_=e;currentData_.clear();}
void ZipOutputStream::closeEntry(){ensureOpen_();if(current_==nullptr)throw ZipException("no current entry");Record r;r.entry=current_;r.data.swap(currentData_);r.crc=crc32_(r.data);r.entry->size_=static_cast<::jxx::lang::jlong>(r.data.size());r.entry->compressedSize_=r.entry->size_;r.entry->crc_=r.crc;r.entry->method_=ZipEntry::STORED_;records_.push_back(std::move(r));current_=nullptr;}
void ZipOutputStream::write(::jxx::lang::jint v){ensureOpen_();if(current_==nullptr)throw ZipException("no current entry");currentData_.push_back(static_cast<std::uint8_t>(v));}
void ZipOutputStream::write(const ::jxx::lang::ByteArray&b){if(b==nullptr)throw ::jxx::lang::NullPointerException();write(b,0,b->length);}
void ZipOutputStream::write(const ::jxx::lang::ByteArray&b,::jxx::lang::jint o,::jxx::lang::jint n){ensureOpen_();if(b==nullptr)throw ::jxx::lang::NullPointerException();if(o<0||n<0||o>b->length-n)throw ::jxx::lang::IndexOutOfBoundsException();if(current_==nullptr)throw ZipException("no current entry");for(int i=0;i<n;++i)currentData_.push_back(static_cast<std::uint8_t>((*b)[o+i]));}
void ZipOutputStream::emit_(std::uint8_t v){out_->write(static_cast<::jxx::lang::jint>(v));++position_;} void ZipOutputStream::emit16_(std::uint16_t v){emit_(v&255);emit_((v>>8)&255);} void ZipOutputStream::emit32_(std::uint32_t v){emit16_(v&65535);emit16_((v>>16)&65535);} void ZipOutputStream::emitBytes_(const std::string&v){for(unsigned char c:v)emit_(c);} void ZipOutputStream::emitBytes_(const std::vector<std::uint8_t>&v){for(auto c:v)emit_(c);}
void ZipOutputStream::finish(){ensureOpen_();if(finished_)return;if(current_!=nullptr)closeEntry();for(auto&r:records_){const auto n=r.entry->getName()->utf8();r.offset=position_;emit32_(0x04034b50);emit16_(20);emit16_(0x0800);emit16_(0);emit16_(0);emit16_(0);emit32_(r.crc);emit32_(static_cast<std::uint32_t>(r.data.size()));emit32_(static_cast<std::uint32_t>(r.data.size()));emit16_(static_cast<std::uint16_t>(n.size()));emit16_(0);emitBytes_(n);emitBytes_(r.data);}const auto central=position_;for(const auto&r:records_){const auto n=r.entry->getName()->utf8();emit32_(0x02014b50);emit16_(20);emit16_(20);emit16_(0x0800);emit16_(0);emit16_(0);emit16_(0);emit32_(r.crc);emit32_(static_cast<std::uint32_t>(r.data.size()));emit32_(static_cast<std::uint32_t>(r.data.size()));emit16_(static_cast<std::uint16_t>(n.size()));emit16_(0);emit16_(0);emit16_(0);emit16_(0);emit32_(r.entry->isDirectory()?0x10U:0);emit32_(r.offset);emitBytes_(n);}const auto centralSize=position_-central;const auto comment=comment_?comment_->utf8():std::string();emit32_(0x06054b50);emit16_(0);emit16_(0);emit16_(static_cast<std::uint16_t>(records_.size()));emit16_(static_cast<std::uint16_t>(records_.size()));emit32_(centralSize);emit32_(central);emit16_(static_cast<std::uint16_t>(comment.size()));emitBytes_(comment);out_->flush();finished_=true;}
void ZipOutputStream::close(){if(closed_)return;finish();out_->close();closed_=true;}
void ZipOutputStream::setMethod(::jxx::lang::jint m){if(m!=ZipEntry::STORED_&&m!=ZipEntry::DEFLATED_)throw ::jxx::lang::IllegalArgumentException();method_=m;} void ZipOutputStream::setLevel(::jxx::lang::jint l){if(l<-1||l>9)throw ::jxx::lang::IllegalArgumentException();level_=l;} void ZipOutputStream::setComment(const ::jxx::Ptr<::jxx::lang::String>&c){if(c&&c->utf8().size()>65535)throw ::jxx::lang::IllegalArgumentException();comment_=c;}
}
