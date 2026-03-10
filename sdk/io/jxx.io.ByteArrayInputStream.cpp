
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.IOException.h"

namespace jxx { namespace io {
ByteArrayInputStream::ByteArrayInputStream(const ByteArray& b): buf(std::make_shared<ByteArray>(b)), pos(0), markpos(0), count(b.size()){}
ByteArrayInputStream::ByteArrayInputStream(std::shared_ptr<const ByteArray> bsp): buf(std::move(bsp)), pos(0), markpos(0), count(buf?buf->size():0){}
int ByteArrayInputStream::read(){ if(pos>=count) return -1; return (int)(*buf)[pos++]; }
int ByteArrayInputStream::read(ByteArray& b, int off, int len){ if(len==0) return 0; if(pos>=count) return -1; size_t avail=count-pos; size_t toCopy=std::min<size_t>(avail,(size_t)len); if(b.size()< (size_t)off + toCopy) throw IOException("IndexOutOfBounds in ByteArrayInputStream.read"); std::memcpy(b.data()+off, buf->data()+pos, toCopy); pos+=toCopy; return (int)toCopy; }
int ByteArrayInputStream::available(){ return (int)std::min<size_t>(count-pos, std::numeric_limits<int>::max()); }
}}
