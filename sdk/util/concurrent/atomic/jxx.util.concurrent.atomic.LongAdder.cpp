#include "util/concurrent/atomic/jxx.util.concurrent.atomic.LongAdder.h"
#include <string>
#include "lang/jxx.lang.String.h"
namespace jxx::util::concurrent::atomic {
::jxx::Ptr<::jxx::lang::ClassAny> LongAdder::Class(){return JxxClassInfoMarker::Class();} LongAdder::LongAdder():Super(){}
void LongAdder::add(::jxx::lang::jlong x)noexcept{value_.fetch_add(x,std::memory_order_relaxed);} void LongAdder::increment()noexcept{add(1);} void LongAdder::decrement()noexcept{add(-1);} ::jxx::lang::jlong LongAdder::sum()const noexcept{return value_.load(std::memory_order_relaxed);} void LongAdder::reset()noexcept{value_.store(0,std::memory_order_relaxed);} ::jxx::lang::jlong LongAdder::sumThenReset()noexcept{return value_.exchange(0,std::memory_order_relaxed);}
::jxx::lang::jint LongAdder::intValue()const{return static_cast<::jxx::lang::jint>(sum());}::jxx::lang::jlong LongAdder::longValue()const{return sum();}::jxx::lang::jfloat LongAdder::floatValue()const{return static_cast<::jxx::lang::jfloat>(sum());}::jxx::lang::jdouble LongAdder::doubleValue()const{return static_cast<::jxx::lang::jdouble>(sum());}::jxx::Ptr<::jxx::lang::String>LongAdder::toString()const{return ::jxx::NEW<::jxx::lang::String>(std::to_string(sum()));}void LongAdder::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&o){(void)o;}void LongAdder::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&i){(void)i;}void LongAdder::readObjectNoData(){}
}
