#include "util/zip/jxx.util.zip.DeflaterOutputStream.h"

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "io/jxx.io.IOException.h"

namespace jxx::util::zip {
namespace { constexpr ::jxx::lang::jint DEFAULT_BUFFER_SIZE = 512; }

DeflaterOutputStream::DeflaterOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output)
    : DeflaterOutputStream(output, ::jxx::NEW<Deflater>(), DEFAULT_BUFFER_SIZE, false) { usesDefaultDeflater_ = true; }
DeflaterOutputStream::DeflaterOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output,const ::jxx::Ptr<Deflater>& deflater)
    : DeflaterOutputStream(output, deflater, DEFAULT_BUFFER_SIZE, false) {}
DeflaterOutputStream::DeflaterOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output,::jxx::lang::jbool syncFlush)
    : DeflaterOutputStream(output, ::jxx::NEW<Deflater>(), DEFAULT_BUFFER_SIZE, syncFlush) { usesDefaultDeflater_ = true; }
DeflaterOutputStream::DeflaterOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output,const ::jxx::Ptr<Deflater>& deflater,::jxx::lang::jint size)
    : DeflaterOutputStream(output, deflater, size, false) {}
DeflaterOutputStream::DeflaterOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output,const ::jxx::Ptr<Deflater>& deflater,::jxx::lang::jint size,::jxx::lang::jbool syncFlush)
    : Super(output), def(deflater), syncFlush_(syncFlush) {
    if(def==nullptr) throw ::jxx::lang::NullPointerException();
    if(size<=0) throw ::jxx::lang::IllegalArgumentException();
    buf=::jxx::NEW<::jxx::lang::ByteArrayType>(size);
}
DeflaterOutputStream::~DeflaterOutputStream(){ if(usesDefaultDeflater_ && def!=nullptr) def->end(); }
void DeflaterOutputStream::ensureOpen_()const{if(closed_)throw ::jxx::io::IOException("stream closed");}
void DeflaterOutputStream::write(::jxx::lang::jint value){auto one=::jxx::NEW<::jxx::lang::ByteArrayType>(1);(*one)[0]=static_cast<::jxx::lang::jbyte>(value);write(one,0,1);}
void DeflaterOutputStream::write(const ::jxx::lang::ByteArray& b){if(b==nullptr)throw ::jxx::lang::NullPointerException();write(b,0,b->length);}
void DeflaterOutputStream::write(const ::jxx::lang::ByteArray& b,::jxx::lang::jint off,::jxx::lang::jint len){
 ensureOpen_();if(b==nullptr)throw ::jxx::lang::NullPointerException();if(off<0||len<0||off>b->length-len)throw ::jxx::lang::ArrayIndexOutOfBoundsException();if(def->finished())throw ::jxx::io::IOException("write beyond end of stream");
 def->setInput(b,off,len);while(!def->needsInput())deflate();
}
void DeflaterOutputStream::deflate(){const auto count=def->deflate(buf,0,buf->length);if(count>0)out_->write(buf,0,count);}
void DeflaterOutputStream::finish(){ensureOpen_();if(finished_)return;def->finish();while(!def->finished()){const auto count=def->deflate(buf,0,buf->length);if(count>0)out_->write(buf,0,count);else if(def->needsInput()&&!def->finished())continue;}finished_=true;}
void DeflaterOutputStream::close(){if(closed_)return;try{finish();out_->close();}catch(...){closed_=true;if(usesDefaultDeflater_)def->end();throw;}closed_=true;if(usesDefaultDeflater_)def->end();}
void DeflaterOutputStream::flush(){ensureOpen_();if(syncFlush_&&!finished_){while(true){const auto count=def->deflate(buf,0,buf->length,Deflater::SYNC_FLUSH);if(count>0)out_->write(buf,0,count);if(count<buf->length)break;}}out_->flush();}
} // namespace jxx::util::zip
