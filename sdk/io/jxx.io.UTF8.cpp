
#include "io/jxx.io.UTF8.h"
namespace jxx { namespace io {
static inline void append_utf8(std::vector<std::uint8_t>& out, uint32_t cp){ if(cp<=0x7F){ out.push_back((uint8_t)cp);} else if(cp<=0x7FF){ 
	out.push_back((uint8_t)(0xC0|((cp>>6)&0x1F))); out.push_back((uint8_t)(0x80|(cp&0x3F))); } else if(cp<=0xFFFF){ out.push_back((uint8_t)(0xE0|((cp>>12)&0x0F)));
	out.push_back((uint8_t)(0x80|((cp>>6)&0x3F))); out.push_back((uint8_t)(0x80|(cp&0x3F))); } else { out.push_back((uint8_t)(0xF0|((cp>>18)&0x07))); 
out.push_back((uint8_t)(0x80|((cp>>12)&0x3F))); out.push_back((uint8_t)(0x80|((cp>>6)&0x3F))); out.push_back((uint8_t)(0x80|(cp&0x3F))); } }
shared_ptr<ByteArray> UTF8::encode(const std::u16string& s){
	std::vector<uint8_t> out; out.reserve(s.size()*2); 
	size_t i=0; while(i<s.size()){ uint16_t u=(uint16_t)s[i++];
if(u>=0xD800 && u<=0xDBFF){ if(i<s.size()){
	uint16_t u2=(uint16_t)s[i]; if(u2>=0xDC00 && u2<=0xDFFF){ ++i;
	uint32_t cp=0x10000 + (((u-0xD800)<<10)|(u2-0xDC00));
append_utf8(out, cp); continue;} }
append_utf8(out, 0xFFFD);} else if(u>=0xDC00 && u<=0xDFFF){
	append_utf8(out, 0xFFFD);} else { append_utf8(out, u);} }
return JXX_NEW<ByteArray>(out.data(), out.size()); 
}
std::u16string UTF8::decode(const ByteArray& bytes){ std::u16string out; size_t i=0,n=bytes.size(); while(i<n){ uint8_t b0=bytes[i++]; uint32_t cp=0; int trail=0;
if((b0&0x80)==0){ cp=b0;} else if((b0&0xE0)==0xC0){ cp=b0&0x1F; trail=1;} else if((b0&0xF0)==0xE0){ cp=b0&0x0F; trail=2;} else if((b0&0xF8)==0xF0){ cp=b0&0x07; trail=3;} 
else { cp=0xFFFD; trail=0;} for(int t=0;t<trail;++t){ if(i>=n){ cp=0xFFFD; break;} uint8_t bx=bytes[i++]; if((bx&0xC0)!=0x80){ cp=0xFFFD; break;} cp=(cp<<6)|(bx&0x3F);} 
if(cp<=0xFFFF){ out.push_back((char16_t)cp);} else { cp-=0x10000; char16_t hi=(char16_t)(0xD800|((cp>>10)&0x3FF)); char16_t lo=(char16_t)(0xDC00|(cp&0x3FF)); out.push_back(hi);
out.push_back(lo);} } return out; }
}}
