#include "util/concurrent/atomic/jxx.util.concurrent.atomic.LongAccumulator.h"
#include <string>
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.String.h"
namespace jxx::util::concurrent::atomic {
::jxx::Ptr<::jxx::lang::ClassAny>LongAccumulator::Class(){return JxxClassInfoMarker::Class();}
LongAccumulator::LongAccumulator(const ::jxx::Ptr<::jxx::util::function::LongBinaryOperator>&f,::jxx::lang::jlong i):Super(),function_(f),identity_(i),value_(i){if(!function_)throw ::jxx::lang::NullPointerException();}
void LongAccumulator::accumulate(::jxx::lang::jlong x){std::lock_guard<std::mutex>l(mutex_);value_=function_->applyAsLong(value_,x);}::jxx::lang::jlong LongAccumulator::get()const{std::lock_guard<std::mutex>l(mutex_);return value_;}void LongAccumulator::reset(){std::lock_guard<std::mutex>l(mutex_);value_=identity_;}::jxx::lang::jlong LongAccumulator::getThenReset(){std::lock_guard<std::mutex>l(mutex_);auto v=value_;value_=identity_;return v;}
::jxx::lang::jint LongAccumulator::intValue()const{return static_cast<::jxx::lang::jint>(get());}::jxx::lang::jlong LongAccumulator::longValue()const{return static_cast<::jxx::lang::jlong>(get());}::jxx::lang::jfloat LongAccumulator::floatValue()const{return static_cast<::jxx::lang::jfloat>(get());}::jxx::lang::jdouble LongAccumulator::doubleValue()const{return static_cast<::jxx::lang::jdouble>(get());}::jxx::Ptr<::jxx::lang::String>LongAccumulator::toString()const{return ::jxx::NEW<::jxx::lang::String>(std::to_string(get()));}void LongAccumulator::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&o){(void)o;}void LongAccumulator::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&i){(void)i;}void LongAccumulator::readObjectNoData(){}
}
