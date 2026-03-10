
#include "io/jxx.io.BufferedInputStream.h"
#include <cstring>
#include <algorithm>
namespace jxx { namespace io {

BufferedInputStream::BufferedInputStream(std::shared_ptr<InputStream> in_, size_t size): FilterInputStream(std::move(in_)), buffer(size) {}

int BufferedInputStream::read(){ if(pos>=count){ if(fill()==-1) return -1;} return buffer[pos++]; }

int BufferedInputStream::read(ByteArray& b, int off, int len)
{
	if(len==0) return 0; int total=0; while(len>0){ if(pos<count){ size_t avail=count-pos; size_t toCopy=std::min<size_t>(avail,(size_t)len);
	std::memcpy(b.data()+off, buffer.data()+pos, toCopy); pos+=toCopy; off+=(int)toCopy; len-=(int)toCopy; total+=(int)toCopy; return total; } 
	if(fill()==-1) break;} return (total==0)?-1:total; } int BufferedInputStream::available(){ int a=(int)(count-pos); int b=in->available();
	return a+b;
}

void BufferedInputStream::mark(int readlimit_)
{ 
	marklimit=readlimit_; markpos=pos;
}

void BufferedInputStream::reset()
{
	if(markpos==(size_t)-1) throw IOException("BufferedInputStream reset() without mark()"); pos=markpos;
}

int BufferedInputStream::fill()
{ 
	if(markpos==(size_t)-1 || (pos-markpos >= (size_t)marklimit)){ 
		markpos=(size_t)-1; pos=0; count=0; 
	}
	else if(markpos>0){ 
		size_t preserved=count-markpos; 
		std::memmove(buffer.data(), buffer.data()+markpos, preserved); 
		pos-=markpos; count=preserved; markpos=0;
	}
	
	ByteArray tmp(buffer.size()-count); 
	int n=in->read(tmp,0,(int)tmp.size()); 
	if(n<=0) return -1; 
	std::memcpy(buffer.data()+count, tmp.data(), (size_t)n); count += (size_t)n; 
	return n;
}
}}
