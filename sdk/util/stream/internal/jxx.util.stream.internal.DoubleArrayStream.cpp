#include <unordered_set>
#include "util/stream/internal/jxx.util.stream.internal.DoubleArrayStream.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::util::stream::internal {
DoubleArrayStream::DoubleArrayStream(const ::jxx::lang::DoubleArray& v):data_(v){}
::jxx::Ptr<::jxx::util::stream::DoubleStream> DoubleArrayStream::sequential(){parallel_=false;return ::jxx::CAST<::jxx::util::stream::DoubleStream>(thisPtr());}
::jxx::Ptr<::jxx::util::stream::DoubleStream> DoubleArrayStream::parallel(){parallel_=true;return ::jxx::CAST<::jxx::util::stream::DoubleStream>(thisPtr());}
::jxx::Ptr<::jxx::util::stream::DoubleStream> DoubleArrayStream::unordered(){return ::jxx::CAST<::jxx::util::stream::DoubleStream>(thisPtr());}
::jxx::Ptr<::jxx::util::stream::DoubleStream> DoubleArrayStream::onClose(const ::jxx::Ptr<::jxx::lang::Runnable>& h){if(!h)throw ::jxx::lang::NullPointerException();closeHandler_=h;return ::jxx::CAST<::jxx::util::stream::DoubleStream>(thisPtr());}
void DoubleArrayStream::close(){if(closed_)return;closed_=true;if(closeHandler_)closeHandler_->run();}
::jxx::Ptr<::jxx::util::stream::DoubleStream> DoubleArrayStream::distinct(){std::unordered_set<::jxx::lang::jdouble> seen; auto out=::jxx::NEW<::jxx::lang::DoubleArrayType>(data_->length); std::uint32_t n=0; for(std::uint32_t i=0;i<data_->length;++i)if(seen.insert((*data_)[i]).second)(*out)[n++]=(*data_)[i]; auto exact=::jxx::NEW<::jxx::lang::DoubleArrayType>(n);for(std::uint32_t i=0;i<n;++i)(*exact)[i]=(*out)[i];return ::jxx::NEW<DoubleArrayStream>(exact);}
::jxx::lang::DoubleArray DoubleArrayStream::toArray(){auto out=::jxx::NEW<::jxx::lang::DoubleArrayType>(data_->length);for(std::uint32_t i=0;i<data_->length;++i)(*out)[i]=(*data_)[i];return out;}
::jxx::lang::jlong DoubleArrayStream::count(){return data_->length;}
::jxx::lang::jdouble DoubleArrayStream::sum(){::jxx::lang::jdouble v=0;for(std::uint32_t i=0;i<data_->length;++i)v+=(*data_)[i];return v;}
}
