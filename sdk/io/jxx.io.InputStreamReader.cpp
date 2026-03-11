
#include "io/jxx.io.InputStreamReader.h"
#include "io/jxx.io.IOException.h"
namespace jxx { namespace io {
InputStreamReader::InputStreamReader(std::shared_ptr<InputStream> in_, const std::string& cs): 
	in(std::move(in_)), charset(cs), byteBuf(4096), pos(0), limit(0), eof(false), hasPending(false), pending(0){}
bool InputStreamReader::fill(){ 
	if(eof) return false; 
	pos=0; limit=0;
	int n=in->read(byteBuf,0,(int)byteBuf.size());
	if(n<=0){ eof=true; 
	return false;
	} 
	limit=(size_t)n;
	return true; 
}
int InputStreamReader::decodeOne(){ 
	if(hasPending){
		hasPending=false; 
		return (int)pending;
	}
	
	while(true){ 
		if(pos>=limit){ 
			if(!fill()) return -1; 
		} 
	ByteArray slice; 
	slice.reserve(4); slice.push_back(byteBuf[pos++]); auto needMore=[&](uint8_t first)->int{ if((first&0x80)==0) return 0; 
	if((first&0xE0)==0xC0) return 1; if((first&0xF0)==0xE0) return 2; if((first&0xF8)==0xF0) return 3; return 0; };
	int cont=needMore(slice[0]); while(cont-- > 0){
		if(pos>=limit){ if(!fill()) break;
	}
	
	slice.push_back(byteBuf[pos++]); 
	} std::u16string u=UTF8::decode(slice); if(u.empty()) return -1; 
if(u.size()==1) return (int)u[0]; pending=u[1]; hasPending=true; return (int)u[0]; } }
int InputStreamReader::read(){ return decodeOne(); }
int InputStreamReader::read(CharArray& buf, int off, int len){ if(off<0||len<0) throw IOException("InputStreamReader.read: invalid args"); 
int i=0; for(;i<len;++i){ int c=decodeOne(); if(c<0) break; buf[off+i]=(jxx::lang::jchar)c; } return (i==0)?-1:i;
}
bool InputStreamReader::ready(){ return (limit - pos) > 0; }
void InputStreamReader::close(){ if(in) in->close(); }
}}
