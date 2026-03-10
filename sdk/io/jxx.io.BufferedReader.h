
#pragma once
#include <memory>
#include <vector>
#include "lang/jxx.lang.h"
#include "io/jxx.io.Reader.h"
namespace jxx { namespace io {
class BufferedReader : public Reader 
{ 
std::shared_ptr<Reader> in; 
CharArray buf; 
int pos=0; 
int limit=0; 
int markPos=-1; 
int readAheadLimit=0; 
public:
    explicit BufferedReader(std::shared_ptr<Reader> in_, int size=8192);
    virtual int read() override; 
    virtual int read(CharArray& cbuf, int off, int len) override;
    virtual std::u16string readLine(); bool ready() override; void close() override; 
    virtual void mark(int readAheadLimit_) override; void reset() override; 
    virtual bool markSupported() override { return true; }
private: int fill(); };
}}
