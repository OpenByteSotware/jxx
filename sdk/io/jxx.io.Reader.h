
#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Object.h"
#include "io/jxx.io.Closeable.h"

namespace jxx { namespace io {
class Reader : public jxx::lang::Object, public Closeable { 
public:
    virtual ~Reader() = default; 
    virtual jxx::lang::jint read() = 0; 
    virtual jxx::lang::jint read(const jxx::lang::CharArray buf, jxx::lang::jint off, jxx::lang::jint len);
    virtual void close() override;
    virtual jxx::lang::jlong skip(jxx::lang::jlong n);
    virtual jxx::lang::jbool ready();
    virtual jxx::lang::jbool markSupported();
    virtual void mark(jxx::lang::jint);
    virtual void reset();
};
}}

