#pragma once
#include "io/jxx.io.Closeable.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"
namespace jxx::io {
class InputStream : public ::jxx::lang::ClassBase<InputStream, ::jxx::lang::Object, Closeable> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<InputStream, JxxSuper, Closeable>;
    ~InputStream() override = default;
    virtual ::jxx::lang::jint read() = 0;
    virtual ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer);
    virtual ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer, ::jxx::lang::jint offset, ::jxx::lang::jint length);
    virtual ::jxx::lang::jlong skip(::jxx::lang::jlong count);
    virtual ::jxx::lang::jint available();
    void close() override;
    virtual void mark(::jxx::lang::jint readLimit);
    virtual void reset();
    virtual ::jxx::lang::jbool markSupported() const;
protected:
    InputStream() = default;
};
} // namespace jxx::io
