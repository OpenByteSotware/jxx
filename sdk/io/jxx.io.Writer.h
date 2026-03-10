
#pragma once
#include <string>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.Flushable.h"
#include "io/jxx.io.IOException.h"
namespace jxx { namespace io {
class Writer : public jxx::lang::Object, public Closeable, public Flushable { public:
    virtual ~Writer() = default; virtual void write(int c) = 0; 
    virtual void write(const jxx::lang::jchar* buf, int off, int len)
    { 
        if(!buf||off<0||len<0) throw IOException("Writer.write: invalid args"); for(int i=0;i<len;++i) write(buf[off+i]); } 
    virtual void write(const std::u16string& s){ if(!s.empty()) write(s.data(),0,(int)s.size()); }
    virtual void flush() override {} virtual void close() override {} };
}}
