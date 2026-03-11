
#include <limits>
#include <iostream>
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "io/jxx.io.IOException.h"
namespace jxx { namespace io {
FileInputStream::FileInputStream(const std::string& path){ ifs.open(path, std::ios::binary); if(!ifs.is_open()) throw FileNotFoundException("FileInputStream: cannot open "+path);} 
FileInputStream::FileInputStream(const FileDescriptor& fd){
    switch(fd.kind()){
        case FileDescriptor::STDIN: ext = &std::cin; break;
        default: throw IOException("FileInputStream: unsupported FileDescriptor kind for input");
    }
}
int FileInputStream::read(){
    if(ext){ int ch = ext->get(); if(ch==EOF) return -1; return (unsigned char)ch; }
    char c; ifs.read(&c,1); if(!ifs) return -1; return (unsigned char)c;
}
jxx::lang::jint FileInputStream::read(ByteArray& b, jxx::lang::jint off, jxx::lang::jint len){
    if(b.size()< (size_t)off + (size_t)len) throw IOException("IndexOutOfBounds in FileInputStream.read");
    if(len<=0) return 0;
    if(ext){ ext->read((char*)(b.data()+off), len); auto g = (jxx::lang::jint)ext->gcount(); if(g==0 && ext->eof()) return -1; return g; }
    ifs.read((char*)(b.data()+off), len); auto g=ifs.gcount(); if(g==0 && ifs.eof()) return -1; return (jxx::lang::jint)g;
}
int FileInputStream::available(){
    if(ext){ return 0; }
    auto cur=ifs.tellg(); if(cur<0) return 0; ifs.seekg(0,std::ios::end); auto end=ifs.tellg(); ifs.seekg(cur); if(end<0) return 0; auto rem=end-cur; if(rem > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max(); return (int)rem;
}
void FileInputStream::close(){ if(ext) return; if(ifs.is_open()) ifs.close(); }
}} // ns
