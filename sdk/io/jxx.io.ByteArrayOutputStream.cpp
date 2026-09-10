#include "io/jxx.io.ByteArrayOutputStream.h"
#include <algorithm>
#include "io/jxx.io.IOHelper.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
namespace jxx::io {
ByteArrayOutputStream::ByteArrayOutputStream():ByteArrayOutputStream(32){} ByteArrayOutputStream::ByteArrayOutputStream(::jxx::lang::jint s){if(s<0)throw ::jxx::lang::IllegalArgumentException();buffer_=::jxx::NEW<::jxx::lang::ByteArrayType>(static_cast<std::uint32_t>(s));}
void ByteArrayOutputStream::ensureCapacity(::jxx::lang::jint c){if(c<=static_cast<::jxx::lang::jint>(buffer_->length))return;auto n=std::max(c,static_cast<::jxx::lang::jint>(buffer_->length)*2);auto b=::jxx::NEW<::jxx::lang::ByteArrayType>(static_cast<std::uint32_t>(n));for(::jxx::lang::jint i=0;i<count_;++i)(*b)[i]=(*buffer_)[i];buffer_=b;}
void ByteArrayOutputStream::write(::jxx::lang::jint v){ensureCapacity(count_+1);(*buffer_)[count_++]=static_cast<::jxx::lang::jbyte>(v);}
void ByteArrayOutputStream::write(const ::jxx::lang::ByteArray& b,::jxx::lang::jint o,::jxx::lang::jint l){IOHelper::checkBounds(b,o,l);ensureCapacity(count_+l);for(::jxx::lang::jint i=0;i<l;++i)(*buffer_)[count_+i]=(*b)[o+i];count_+=l;}
void ByteArrayOutputStream::writeTo(const ::jxx::Ptr<OutputStream>& o)const{if(!o)throw ::jxx::lang::NullPointerException();o->write(buffer_,0,count_);} void ByteArrayOutputStream::reset(){count_=0;} ::jxx::lang::ByteArray ByteArrayOutputStream::toByteArray()const{auto b=::jxx::NEW<::jxx::lang::ByteArrayType>(static_cast<std::uint32_t>(count_));for(::jxx::lang::jint i=0;i<count_;++i)(*b)[i]=(*buffer_)[i];return b;} ::jxx::lang::jint ByteArrayOutputStream::size()const{return count_;} ::jxx::Ptr<::jxx::lang::String> ByteArrayOutputStream::toString()const{return ::jxx::NEW<::jxx::lang::String>(toByteArray());} void ByteArrayOutputStream::close(){}
} // namespace jxx::io
