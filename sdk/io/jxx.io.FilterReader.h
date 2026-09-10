#pragma once
#include "io/jxx.io.Reader.h"
namespace jxx::io {
class FilterReader : public ::jxx::lang::ClassBase<FilterReader,Reader> {
public:
 using JxxSuper=Reader; using Super=::jxx::lang::ClassBase<FilterReader,JxxSuper>;
 ~FilterReader() override=default;
 ::jxx::lang::jint read() override; ::jxx::lang::jint read(const ::jxx::lang::CharArray& buffer,::jxx::lang::jint offset,::jxx::lang::jint length) override; ::jxx::lang::jlong skip(::jxx::lang::jlong count) override; ::jxx::lang::jbool ready() override; ::jxx::lang::jbool markSupported()const override; void mark(::jxx::lang::jint limit) override; void reset() override; void close() override;
protected: explicit FilterReader(const ::jxx::Ptr<Reader>& input); ::jxx::Ptr<Reader> in_;
};
} // namespace jxx::io
