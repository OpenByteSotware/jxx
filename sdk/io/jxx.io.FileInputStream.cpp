
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "io/jxx.io.IOException.h"

namespace jxx { namespace io {
FileInputStream::FileInputStream(const std::string& path){ ifs.open(path, std::ios::binary); if(!ifs.is_open()) throw FileNotFoundException("FileInputStream: cannot open "+path);} 
int FileInputStream::read(){ char c; ifs.read(&c,1); if(!ifs) return -1; return (unsigned char)c; }
int FileInputStream::read(ByteArray& b, int off, int len){ if(b.size()< (size_t)off + (size_t)len) throw IOException("IndexOutOfBounds in FileInputStream.read"); ifs.read((char*)(b.data()+off), len); auto g=ifs.gcount(); if(g==0 && ifs.eof()) return -1; return (int)g; }
int FileInputStream::available(){ auto cur=ifs.tellg(); if(cur<0) return 0; ifs.seekg(0,std::ios::end); auto end=ifs.tellg(); ifs.seekg(cur); if(end<0) return 0; auto rem=end-cur; if(rem > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max(); return (int)rem; }
void FileInputStream::close(){ if(ifs.is_open()) ifs.close(); }
}}
