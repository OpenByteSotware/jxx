
#include "io/jxx.io.FileOutputStream.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "io/jxx.io.IOException.h"
#include <iostream>
namespace jxx { namespace io {
FileOutputStream::FileOutputStream(const std::string& path, bool append){ ofs.open(path, std::ios::binary | (append? std::ios::app : std::ios::trunc)); if(!ofs.is_open()) throw FileNotFoundException("FileOutputStream: cannot open "+path);} 
FileOutputStream::FileOutputStream(const FileDescriptor& fd){
    switch(fd.kind()){
        case FileDescriptor::STDOUT: ext = &std::cout; break;
        case FileDescriptor::STDERR: ext = &std::cerr; break;
        default: throw IOException("FileOutputStream: unsupported FileDescriptor kind for output");
    }
}
void FileOutputStream::write(int b){
    if(ext){ char c=(char)(b&0xFF); ext->write(&c,1); if(!(*ext)) throw IOException("FileOutputStream.write failed"); return; }
    char c=(char)(b&0xFF); ofs.write(&c,1); if(!ofs) throw IOException("FileOutputStream.write failed");
}
void FileOutputStream::write(const ByteArray& b, jxx::lang::jint off, jxx::lang::jint len){
    if(b.size()< (size_t)off + (size_t)len) throw IOException("IndexOutOfBounds in FileOutputStream.write");
    if(ext){ ext->write((const char*)(b.data()+off), len); if(!(*ext)) throw IOException("FileOutputStream.write failed"); return; }
    ofs.write((const char*)(b.data()+off), len); if(!ofs) throw IOException("FileOutputStream.write failed");
}
void FileOutputStream::flush(){ if(ext){ ext->flush(); return; } ofs.flush(); }
void FileOutputStream::close(){ if(ext) return; if(ofs.is_open()) ofs.close(); }
}} // ns
