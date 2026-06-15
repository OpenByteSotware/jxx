#include <vector>
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.internal.h"
#include "jxx.io.ModifiedUTF.h"
#include "jxx.io.UTFDataFormatException.h"

using namespace std;

namespace jxx { namespace io {
jxx::lang::ByteArray ModifiedUTF::encode(const std::u16string& s){
	std::vector<jxx::lang::jbyte> out;
	out.reserve(s.size()*3);
	for(char16_t ch: s){ uint16_t c=(uint16_t)ch; 
	if(c>=1 && c<=0x7F){ out.push_back((uint8_t)c);} 
	else if(c==0 || (c>=0x80 && c<=0x07FF)){ 
		out.push_back((uint8_t)(0xC0|((c>>6)&0x1F)));
		out.push_back((uint8_t)(0x80|(c&0x3F))); 
	} else { 
		out.push_back((uint8_t)(0xE0|((c>>12)&0x0F)));
		out.push_back((uint8_t)(0x80|((c>>6)&0x3F))); out.push_back((uint8_t)(0x80|(c&0x3F)));
	}
	} 
	
	return jxx::NEW<jxx::lang::ByteArrayType>(out.data());
}
std::u16string ModifiedUTF::decode(const jxx::lang::ByteArray bytes)
{
	std::u16string out; 
	jxx::lang::jint i=0; 
	while(i<bytes->size()){ 
		uint8_t b=(*bytes)[i++]; 
		if((b&0x80)==0){ 
			out.push_back((char16_t)b);
		} 
		else if((b&0xE0)==0xC0){ 
			if(i>=bytes->size()) throw UTFDataFormatException("Invalid modified UTF-8"); 
			uint8_t b2=(*bytes)[i++]; 
			if((b2&0xC0)!=0x80) throw UTFDataFormatException("Invalid modified UTF-8"); 
			uint16_t ch=(uint16_t)(((b&0x1F)<<6)|(b2&0x3F)); 
			out.push_back((char16_t)ch);
		} 
		else if((b&0xF0)==0xE0){ 
			if(i+1>=bytes->size()) throw UTFDataFormatException("Invalid modified UTF-8");
			uint8_t b2=(*bytes)[i++], b3=(*bytes)[i++]; 
			if(((b2&0xC0)!=0x80)||((b3&0xC0)!=0x80)) throw UTFDataFormatException("Invalid modified UTF-8");
			uint16_t ch=(uint16_t)(((b&0x0F)<<12)|((b2&0x3F)<<6)|(b3&0x3F)); 
			out.push_back((char16_t)ch);
		} 
		else { 
			throw UTFDataFormatException("Invalid modified UTF-8 leading byte"); 
		} 
	}
	return out; 
}
}}
