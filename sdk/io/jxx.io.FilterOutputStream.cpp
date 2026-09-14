#include "io/jxx.io.FilterOutputStream.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::io {
FilterOutputStream::FilterOutputStream(const ::jxx::Ptr<OutputStream>& output):out_(output){if(!out_)throw ::jxx::lang::NullPointerException();}
void FilterOutputStream::write(::jxx::lang::jint v){
    synchronized([&] {out_->write(v);
    });
} void FilterOutputStream::write(const ::jxx::lang::ByteArray& b){
    synchronized([&] {out_->write(b);
    });
} void FilterOutputStream::write(const ::jxx::lang::ByteArray& b,::jxx::lang::jint o,::jxx::lang::jint l){
    synchronized([&] {out_->write(b,o,l);
    });
} void FilterOutputStream::flush(){
    synchronized([&] {out_->flush();
    });
} void FilterOutputStream::close(){
    synchronized([&] {try{flush();}catch(...){out_->close();throw;}out_->close();
    });
}
} // namespace jxx::io
