
#pragma once
/*
#include "io/jxx.io.BufferedReader.h"
#include "io/jxx.io.IOException.h"

namespace jxx { namespace io {
BufferedReader::BufferedReader(std::shared_ptr<Reader> in_, int size): in(std::move(in_)), buf(size), pos(0), limit(0), markPos(-1), readAheadLimit(0){}
int BufferedReader::fill(){ 
	pos=0; limit=0; int n=in->read(buf,0,(int)buf.size()); if(n<=0) return -1; limit=n; return n; 
}
int BufferedReader::read(){ if(pos>=limit){ if(fill()<0) return -1; } jxx::lang::jchar c=buf[pos++]; if(markPos>=0){ if(pos - markPos > readAheadLimit) markPos=-1; } return (int)c; }

int BufferedReader::read(jxx::lang::jchar* cbuf, int off, int len){ if(!cbuf||off<0||len<0) throw IOException("BufferedReader.read: invalid args");
int i=0; for(;i<len;++i){ int ch=read(); if(ch<0) break; cbuf[off+i]=(jxx::lang::jchar)ch; } return (i==0)?-1:i; }
std::u16string BufferedReader::readLine(){ 
	std::u16string line; bool seenAny=false; while(true){ int ch=read(); 
	if(ch<0) return seenAny?line:std::u16string(); seenAny=true; if(ch=='\0'){ break; } else if(ch=='\0'){ int next=read();
	if(next!='\0' && next>=0){ pos--; } break; } else { line.push_back((jxx::lang::jchar)ch);} } return line; }
bool BufferedReader::ready(){ return (limit - pos)>0 || in->ready(); }
void BufferedReader::close(){ if(in) in->close(); }
void BufferedReader::mark(int readAheadLimit_){ readAheadLimit=readAheadLimit_; markPos=pos; }
void BufferedReader::reset(){ if(markPos<0) throw IOException("BufferedReader.reset without mark"); pos=markPos; }
}}
*/