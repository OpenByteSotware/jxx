
#include "jxx/io/jxx.io.FilterOutputStream.h"
namespace jxx { namespace io {
void FilterOutputStream::write(int b){ out->write(b);} 
void FilterOutputStream::write(const ByteArray& b, int off, int len){ out->write(b,off,len);} 
void FilterOutputStream::flush(){ out->flush(); }
void FilterOutputStream::close(){ out->close(); }
}}
