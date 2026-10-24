#include <unordered_set>
#include "util/stream/internal/jxx.util.stream.internal.LongArrayStream.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::util::stream::internal {
LongArrayStream::LongArrayStream(const ::jxx::lang::LongArray& v):data_(v){}
::jxx::Ptr<::jxx::util::stream::LongStream> LongArrayStream::sequential(){parallel_=false;return ::jxx::CAST<::jxx::util::stream::LongStream>(thisPtr());}
::jxx::Ptr<::jxx::util::stream::LongStream> LongArrayStream::parallel(){parallel_=true;return ::jxx::CAST<::jxx::util::stream::LongStream>(thisPtr());}
::jxx::Ptr<::jxx::util::stream::LongStream> LongArrayStream::unordered(){return ::jxx::CAST<::jxx::util::stream::LongStream>(thisPtr());}
::jxx::Ptr<::jxx::util::stream::LongStream> LongArrayStream::onClose(const ::jxx::Ptr<::jxx::lang::Runnable>& h){if(!h)throw ::jxx::lang::NullPointerException();closeHandler_=h;return ::jxx::CAST<::jxx::util::stream::LongStream>(thisPtr());}
void LongArrayStream::close(){if(closed_)return;closed_=true;if(closeHandler_)closeHandler_->run();}
::jxx::Ptr<::jxx::util::stream::LongStream> LongArrayStream::distinct(){std::unordered_set<::jxx::lang::jlong> seen; auto out=::jxx::NEW<::jxx::lang::LongArrayType>(data_->length); std::uint32_t n=0; for(std::uint32_t i=0;i<data_->length;++i)if(seen.insert((*data_)[i]).second)(*out)[n++]=(*data_)[i]; auto exact=::jxx::NEW<::jxx::lang::LongArrayType>(n);for(std::uint32_t i=0;i<n;++i)(*exact)[i]=(*out)[i];return ::jxx::NEW<LongArrayStream>(exact);}
::jxx::lang::LongArray LongArrayStream::toArray(){auto out=::jxx::NEW<::jxx::lang::LongArrayType>(data_->length);for(std::uint32_t i=0;i<data_->length;++i)(*out)[i]=(*data_)[i];return out;}
::jxx::lang::jlong LongArrayStream::count(){return data_->length;}
::jxx::lang::jlong LongArrayStream::sum(){::jxx::lang::jlong v=0;for(std::uint32_t i=0;i<data_->length;++i)v+=(*data_)[i];return v;}
}
