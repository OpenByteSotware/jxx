
#include <cstring>
#include "io/jxx.io.PushbackInputStream.h"
#include "io/jxx.io.IOException.h"
namespace jxx { namespace io {
PushbackInputStream::PushbackInputStream(std::shared_ptr<InputStream> in_, int size) : FilterInputStream(std::move(in_)), buf((size_t)size), pos(size) { if(size<=0) throw IOException("PushbackInputStream: size <= 0"); }
int PushbackInputStream::read(){ if(pos< (int)buf.size()) return buf[pos++]; return in->read(); }
int PushbackInputStream::read(ByteArray& b, int off, int len){ if(len==0) return 0; int total=0; while(len>0 && pos<(int)buf.size()){ b[(size_t)off++]=buf[pos++]; --len; ++total; } if(len>0){ auto r=in->read(b,off,len); if(r>0) total+=r; else if(total==0) return r; } return total; }
void PushbackInputStream::unread(int v){ if(pos==0) throw IOException("PushbackInputStream: pushback buffer overflow"); buf[--pos]=(uint8_t)(v&0xFF); }
void PushbackInputStream::unread(const ByteArray& b, int off, int len){ if(len>pos) throw IOException("PushbackInputStream: pushback buffer overflow"); pos-=len; std::memcpy(buf.data()+pos, b.data()+off, (size_t)len); }
void PushbackInputStream::unread(const ByteArray& b){ unread(b,0,(int)b.size()); }
int PushbackInputStream::available(){ int a=(int)buf.size()-pos; return a + in->available(); }
}}
