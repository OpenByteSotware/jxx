#include "io/jxx.io.Reader.h"
#include <algorithm>
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::io {
namespace { void check(const ::jxx::lang::CharArray& b,::jxx::lang::jint o,::jxx::lang::jint l){if(!b)throw ::jxx::lang::NullPointerException();if(o<0||l<0||o>static_cast<::jxx::lang::jint>(b->length)-l)throw ::jxx::lang::IndexOutOfBoundsException();} }
Reader::Reader():lock_(thisPtr()){} Reader::Reader(const ::jxx::Ptr<::jxx::lang::Object>& lock):lock_(lock){if(!lock_)throw ::jxx::lang::NullPointerException();}
::jxx::lang::jint Reader::read(){auto b=::jxx::NEW<::jxx::lang::CharArrayType>(1);auto n=read(b,0,1);return n<0?-1:static_cast<::jxx::lang::jint>((*b)[0]);}
::jxx::lang::jint Reader::read(const ::jxx::lang::CharArray& b){check(b,0,static_cast<::jxx::lang::jint>(b->length));return read(b,0,static_cast<::jxx::lang::jint>(b->length));}
::jxx::lang::jlong Reader::skip(::jxx::lang::jlong n){if(n<0)throw ::jxx::lang::IllegalArgumentException();auto b=::jxx::NEW<::jxx::lang::CharArrayType>(8192);::jxx::lang::jlong total=0;while(total<n){auto amount=static_cast<::jxx::lang::jint>(std::min<::jxx::lang::jlong>(n-total,8192));auto count=read(b,0,amount);if(count<0)break;total+=count;}return total;}
::jxx::lang::jbool Reader::ready(){return false;} ::jxx::lang::jbool Reader::markSupported()const{return false;} void Reader::mark(::jxx::lang::jint){throw IOException();} void Reader::reset(){throw IOException();}
} // namespace jxx::io
