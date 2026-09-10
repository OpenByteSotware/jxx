#pragma once
#include "io/jxx.io.Closeable.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"
namespace jxx::io {
class Reader : public ::jxx::lang::ClassBase<Reader,::jxx::lang::Object,Closeable> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<Reader,JxxSuper,Closeable>;
 ~Reader() override=default;
 virtual ::jxx::lang::jint read();
 virtual ::jxx::lang::jint read(const ::jxx::lang::CharArray& buffer);
 virtual ::jxx::lang::jint read(const ::jxx::lang::CharArray& buffer,::jxx::lang::jint offset,::jxx::lang::jint length)=0;
 virtual ::jxx::lang::jlong skip(::jxx::lang::jlong count);
 virtual ::jxx::lang::jbool ready();
 virtual ::jxx::lang::jbool markSupported() const;
 virtual void mark(::jxx::lang::jint readAheadLimit);
 virtual void reset();
 virtual void close() override=0;
protected:
 Reader(); explicit Reader(const ::jxx::Ptr<::jxx::lang::Object>& lock);
 ::jxx::Ptr<::jxx::lang::Object> lock_;
};
} // namespace jxx::io
