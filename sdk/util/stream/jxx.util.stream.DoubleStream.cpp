#include "util/stream/jxx.util.stream.DoubleStream.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/stream/internal/jxx.util.stream.internal.DoubleArrayStream.h"
namespace jxx::util::stream {
using DoubleImpl=::jxx::util::stream::internal::DoubleArrayStream;
::jxx::Ptr<DoubleStream> DoubleStream::empty(){return ::jxx::NEW<DoubleImpl>(::jxx::NEW<::jxx::lang::DoubleArrayType>(0));}
::jxx::Ptr<DoubleStream> DoubleStream::of(::jxx::lang::jdouble v){auto a=::jxx::NEW<::jxx::lang::DoubleArrayType>(1);(*a)[0]=v;return ::jxx::NEW<DoubleImpl>(a);}
::jxx::Ptr<DoubleStream> DoubleStream::of(const ::jxx::lang::DoubleArray& v){if(!v)throw ::jxx::lang::NullPointerException();return ::jxx::NEW<DoubleImpl>(v);}
}
