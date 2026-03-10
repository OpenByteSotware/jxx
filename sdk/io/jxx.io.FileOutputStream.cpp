
#include "io/jxx.io.FileOutputStream.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "io/jxx.io.IOException.h"

namespace jxx { namespace io {
FileOutputStream::FileOutputStream(const std::string& path, bool append){ ofs.open(path, std::ios::binary | (append? std::ios::app : std::ios::trunc)); if(!ofs.is_open()) throw FileNotFoundException("FileOutputStream: cannot open "+path);} 
void FileOutputStream::write(int b){ char c=(char)(b&0xFF); ofs.write(&c,1); if(!ofs) throw IOException("FileOutputStream.write failed"); }
void FileOutputStream::write(const ByteArray& b, int off, int len){ if(b.size()< (size_t)off + (size_t)len) throw IOException("IndexOutOfBounds in FileOutputStream.write"); ofs.write((const char*)(b.data()+off), len); if(!ofs) throw IOException("FileOutputStream.write failed"); }
void FileOutputStream::flush(){ ofs.flush(); }
void FileOutputStream::close(){ if(ofs.is_open()) ofs.close(); }
}}
