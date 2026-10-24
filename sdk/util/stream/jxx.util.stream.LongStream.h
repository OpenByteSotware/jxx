#pragma once
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx_types.h"
#include "util/stream/jxx.util.stream.BaseStream.h"
namespace jxx::util::stream {
class LongStream : public ::jxx::lang::InterfaceBase<LongStream, BaseStream<::jxx::lang::Long, LongStream>> {
public:
    ~LongStream() override = default;
    static ::jxx::Ptr<LongStream> empty();
    static ::jxx::Ptr<LongStream> of(::jxx::lang::jlong value);
    static ::jxx::Ptr<LongStream> of(const ::jxx::lang::LongArray& values);
    virtual ::jxx::Ptr<LongStream> distinct() = 0;
    virtual ::jxx::lang::LongArray toArray() = 0;
    virtual ::jxx::lang::jlong count() = 0;
    virtual ::jxx::lang::jlong sum() = 0;
};
}
