
#pragma once
#include <memory>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.Writer.h"
#include "io/jxx.io.OutputStream.h"
#include "io/jxx.io.UTF8.h"

namespace jxx { namespace io {
class OutputStreamWriter : public Writer { std::shared_ptr<OutputStream> out; std::string charset; public:
    explicit OutputStreamWriter(std::shared_ptr<OutputStream> out_, const std::string& cs="UTF-8");
    void write(int c) override;
    void write(const jxx::lang::jchar* buf, int off, int len) override; void write(const std::u16string& s) override; void flush() override; void close() override; };
}}
