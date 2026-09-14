#include "io/jxx.io.FilterInputStream.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::io {
FilterInputStream::FilterInputStream(const ::jxx::Ptr<InputStream>& input):in_(input){if(!in_)throw ::jxx::lang::NullPointerException();}
::jxx::lang::jint FilterInputStream::read(){
    return synchronized([&]() -> ::jxx::lang::jint {return in_->read();
    });
} ::jxx::lang::jint FilterInputStream::read(const ::jxx::lang::ByteArray& b){
    return synchronized([&]() -> ::jxx::lang::jint {return in_->read(b);
    });
} ::jxx::lang::jint FilterInputStream::read(const ::jxx::lang::ByteArray& b,::jxx::lang::jint o,::jxx::lang::jint l){
    return synchronized([&]() -> ::jxx::lang::jint {return in_->read(b,o,l);
    });
} ::jxx::lang::jlong FilterInputStream::skip(::jxx::lang::jlong n){
    return synchronized([&]() -> ::jxx::lang::jlong {return in_->skip(n);
    });
} ::jxx::lang::jint FilterInputStream::available(){
    return synchronized([&]() -> ::jxx::lang::jint {return in_->available();
    });
} void FilterInputStream::close(){
    synchronized([&] {in_->close();
    });
} void FilterInputStream::mark(::jxx::lang::jint n){
    synchronized([&] {in_->mark(n);
    });
} void FilterInputStream::reset(){
    synchronized([&] {in_->reset();
    });
} ::jxx::lang::jbool FilterInputStream::markSupported()const{return in_->markSupported();}
} // namespace jxx::io
