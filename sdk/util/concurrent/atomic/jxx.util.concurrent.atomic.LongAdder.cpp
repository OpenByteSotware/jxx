#include "util/concurrent/atomic/jxx.util.concurrent.atomic.LongAdder.h"
#include <string>
#include "lang/jxx.lang.String.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::util::concurrent::atomic {
::jxx::Ptr<::jxx::lang::ClassAny> LongAdder::Class(){return JxxClassInfoMarker::Class();} LongAdder::LongAdder():Super(){}
void LongAdder::add(::jxx::lang::jlong x)noexcept{value_.fetch_add(x,std::memory_order_relaxed);} void LongAdder::increment()noexcept{add(1);} void LongAdder::decrement()noexcept{add(-1);} ::jxx::lang::jlong LongAdder::sum()const noexcept{return value_.load(std::memory_order_relaxed);} void LongAdder::reset()noexcept{value_.store(0,std::memory_order_relaxed);} ::jxx::lang::jlong LongAdder::sumThenReset()noexcept{return value_.exchange(0,std::memory_order_relaxed);}
::jxx::lang::jint LongAdder::intValue()const{return static_cast<::jxx::lang::jint>(sum());}::jxx::lang::jlong LongAdder::longValue()const{return sum();}::jxx::lang::jfloat LongAdder::floatValue()const{return static_cast<::jxx::lang::jfloat>(sum());}::jxx::lang::jdouble LongAdder::doubleValue()const{return static_cast<::jxx::lang::jdouble>(sum());}::jxx::Ptr<::jxx::lang::String>LongAdder::toString()const{return ::jxx::NEW<::jxx::lang::String>(std::to_string(sum()));}void LongAdder::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) {
    if (output == nullptr) throw ::jxx::lang::NullPointerException();
    output->writeLong(sum());
}
void LongAdder::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) {
    if (input == nullptr) throw ::jxx::lang::NullPointerException();
    value_.store(input->readLong(), std::memory_order_relaxed);
}
void LongAdder::readObjectNoData() {
    value_.store(0, std::memory_order_relaxed);
}
}
