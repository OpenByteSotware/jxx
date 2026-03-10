
#pragma once
#include <memory>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.Reader.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.UTF8.h"
namespace jxx { namespace io {
class InputStreamReader : public Reader { std::shared_ptr<InputStream> in; std::string charset; ByteArray byteBuf; size_t pos=0,limit=0; bool eof=false; bool hasPending=false; jxx::lang::jchar pending; public:
    explicit InputStreamReader(std::shared_ptr<InputStream> in_, const std::string& cs="UTF-8"); int read() override; int read(jxx::lang::jchar* buf, int off, int len) override; bool ready() override; void close() override; private: int decodeOne(); bool fill(); };
}}
