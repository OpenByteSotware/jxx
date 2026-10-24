#include "util/stream/jxx.util.stream.IntStream.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/stream/internal/jxx.util.stream.internal.IntArrayStream.h"
namespace jxx::util::stream {
using Impl = ::jxx::util::stream::internal::IntArrayStream;
::jxx::Ptr<IntStream> IntStream::empty() { return ::jxx::NEW<Impl>(::jxx::NEW<::jxx::lang::IntArrayType>(0)); }
::jxx::Ptr<IntStream> IntStream::of(::jxx::lang::jint value) { auto a=::jxx::NEW<::jxx::lang::IntArrayType>(1); (*a)[0]=value; return ::jxx::NEW<Impl>(a); }
::jxx::Ptr<IntStream> IntStream::of(const ::jxx::lang::IntArray& values) { if(!values) throw ::jxx::lang::NullPointerException(); return ::jxx::NEW<Impl>(values); }
::jxx::Ptr<IntStream> IntStream::range(::jxx::lang::jint a, ::jxx::lang::jint b) { if(a>=b) return empty(); auto n=static_cast<std::uint32_t>(static_cast<::jxx::lang::jlong>(b)-a); auto v=::jxx::NEW<::jxx::lang::IntArrayType>(n); for(std::uint32_t i=0;i<n;++i)(*v)[i]=static_cast<::jxx::lang::jint>(a+i); return ::jxx::NEW<Impl>(v); }
::jxx::Ptr<IntStream> IntStream::rangeClosed(::jxx::lang::jint a, ::jxx::lang::jint b) { if(a>b) return empty(); auto n=static_cast<std::uint32_t>(static_cast<::jxx::lang::jlong>(b)-a+1); auto v=::jxx::NEW<::jxx::lang::IntArrayType>(n); for(std::uint32_t i=0;i<n;++i)(*v)[i]=static_cast<::jxx::lang::jint>(a+i); return ::jxx::NEW<Impl>(v); }
::jxx::Ptr<IntStream> IntStream::concat(const ::jxx::Ptr<IntStream>& a,const ::jxx::Ptr<IntStream>& b) { if(!a||!b) throw ::jxx::lang::NullPointerException(); auto x=a->toArray(),y=b->toArray(); auto v=::jxx::NEW<::jxx::lang::IntArrayType>(x->length+y->length); std::uint32_t k=0; for(std::uint32_t i=0;i<x->length;++i)(*v)[k++]=(*x)[i]; for(std::uint32_t i=0;i<y->length;++i)(*v)[k++]=(*y)[i]; return ::jxx::NEW<Impl>(v); }
}
