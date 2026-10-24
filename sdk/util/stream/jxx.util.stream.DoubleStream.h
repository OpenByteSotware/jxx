#pragma once
#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx_types.h"
#include "util/stream/jxx.util.stream.BaseStream.h"
namespace jxx::util::stream {
class DoubleStream : public ::jxx::lang::InterfaceBase<DoubleStream, BaseStream<::jxx::lang::Double, DoubleStream>> {
public:
    ~DoubleStream() override = default;
    static ::jxx::Ptr<DoubleStream> empty();
    static ::jxx::Ptr<DoubleStream> of(::jxx::lang::jdouble value);
    static ::jxx::Ptr<DoubleStream> of(const ::jxx::lang::DoubleArray& values);
    virtual ::jxx::Ptr<DoubleStream> distinct() = 0;
    virtual ::jxx::lang::DoubleArray toArray() = 0;
    virtual ::jxx::lang::jlong count() = 0;
    virtual ::jxx::lang::jdouble sum() = 0;
};
}
