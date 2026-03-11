
#pragma once
#include <string>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.IOException.h"
namespace jxx { namespace io {
class Reader : public jxx::lang::Object, public Closeable { 
public:
    virtual ~Reader() = default; 
    virtual int read() = 0; 
    virtual int read(CharArray& buf, int off, int len){
        if(off<0||len<0) throw IOException("Reader.read: invalid args"); 
        int i=0; for(;i<len;++i){ 
            int c=read();
        if(c<0) break; 
        buf[off+i]=(jxx::lang::jchar)c;}
        return (i==0)?-1:i; 
    } 
    virtual void close() override {} 
    virtual long skip(long n){ long r=0; for(;r<n;++r){ if(read()<0) break;} return r;} 
    virtual bool ready(){ return false;} 
    virtual bool markSupported(){ return false;} 
    virtual void mark(int){ throw IOException("mark not supported"); } 
    virtual void reset(){ throw IOException("reset not supported"); } };
}}
