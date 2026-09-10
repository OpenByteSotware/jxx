#pragma once
#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.Flushable.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"
namespace jxx::io {
class OutputStream : public ::jxx::lang::ClassBase<OutputStream, ::jxx::lang::Object, Closeable, Flushable> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<OutputStream, JxxSuper, Closeable, Flushable>;
    ~OutputStream() override = default;
    virtual void write(::jxx::lang::jint value) = 0;
    virtual void write(const ::jxx::lang::ByteArray& buffer);
    virtual void write(const ::jxx::lang::ByteArray& buffer, ::jxx::lang::jint offset, ::jxx::lang::jint length);
    void flush() override;
    void close() override;
protected:
    OutputStream() = default;
};
} // namespace jxx::io
