#include "util/stream/jxx.util.stream.LongStream.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/stream/internal/jxx.util.stream.internal.LongArrayStream.h"
namespace jxx::util::stream {
using LongImpl=::jxx::util::stream::internal::LongArrayStream;
::jxx::Ptr<LongStream> LongStream::empty(){return ::jxx::NEW<LongImpl>(::jxx::NEW<::jxx::lang::LongArrayType>(0));}
::jxx::Ptr<LongStream> LongStream::of(::jxx::lang::jlong v){auto a=::jxx::NEW<::jxx::lang::LongArrayType>(1);(*a)[0]=v;return ::jxx::NEW<LongImpl>(a);}
::jxx::Ptr<LongStream> LongStream::of(const ::jxx::lang::LongArray& v){if(!v)throw ::jxx::lang::NullPointerException();return ::jxx::NEW<LongImpl>(v);}
}
