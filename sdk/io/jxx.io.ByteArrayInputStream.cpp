#include "io/jxx.io.ByteArrayInputStream.h"
#include <algorithm>
#include "io/jxx.io.IOHelper.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::io {
ByteArrayInputStream::ByteArrayInputStream(const ::jxx::lang::ByteArray& b):ByteArrayInputStream(b,0,b?static_cast<::jxx::lang::jint>(b->length):0){}
ByteArrayInputStream::ByteArrayInputStream(const ::jxx::lang::ByteArray& b,::jxx::lang::jint o,::jxx::lang::jint l):buffer_(b){if(!b)throw ::jxx::lang::NullPointerException();auto size=static_cast<::jxx::lang::jint>(b->length);position_=std::min(o,size);mark_=position_;count_=std::min(o+l,size);}
::jxx::lang::jint ByteArrayInputStream::read(){return position_<count_?(static_cast<::jxx::lang::jint>((*buffer_)[position_++])&0xff):-1;}
::jxx::lang::jint ByteArrayInputStream::read(const ::jxx::lang::ByteArray& b,::jxx::lang::jint o,::jxx::lang::jint l){IOHelper::checkBounds(b,o,l);if(position_>=count_)return -1;auto n=std::min(l,count_-position_);if(n<=0)return 0;for(::jxx::lang::jint i=0;i<n;++i)(*b)[o+i]=(*buffer_)[position_+i];position_+=n;return n;}
::jxx::lang::jlong ByteArrayInputStream::skip(::jxx::lang::jlong n){auto k=std::min<::jxx::lang::jlong>(std::max<::jxx::lang::jlong>(n,0),count_-position_);position_+=static_cast<::jxx::lang::jint>(k);return k;} ::jxx::lang::jint ByteArrayInputStream::available(){return count_-position_;} void ByteArrayInputStream::close(){} void ByteArrayInputStream::mark(::jxx::lang::jint){mark_=position_;} void ByteArrayInputStream::reset(){position_=mark_;} ::jxx::lang::jbool ByteArrayInputStream::markSupported()const{return true;}
} // namespace jxx::io
