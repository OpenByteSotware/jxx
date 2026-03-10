
#include "io/jxx.io.FilterInputStream.h"

namespace jxx { namespace io {
int FilterInputStream::read(){ return in->read(); }
int FilterInputStream::read(ByteArray& b, int off, int len){ return in->read(b,off,len);} 
jxx::lang::jlong FilterInputStream::skip(jxx::lang::jlong n){ return in->skip(n);} 
int FilterInputStream::available(){ return in->available(); }
void FilterInputStream::close(){ in->close(); }
void FilterInputStream::mark(int readlimit){ in->mark(readlimit);} 
void FilterInputStream::reset(){ in->reset(); }
bool FilterInputStream::markSupported(){ return in->markSupported(); }
}}
