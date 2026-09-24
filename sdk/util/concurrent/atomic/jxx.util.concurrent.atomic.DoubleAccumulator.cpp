#include "util/concurrent/atomic/jxx.util.concurrent.atomic.DoubleAccumulator.h"
#include <string>
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.String.h"
namespace jxx::util::concurrent::atomic {
::jxx::Ptr<::jxx::lang::ClassAny>DoubleAccumulator::Class(){return JxxClassInfoMarker::Class();}
DoubleAccumulator::DoubleAccumulator(const ::jxx::Ptr<::jxx::util::function::DoubleBinaryOperator>&f,::jxx::lang::jdouble i):Super(),function_(f),identity_(i),value_(i){if(!function_)throw ::jxx::lang::NullPointerException();}
void DoubleAccumulator::accumulate(::jxx::lang::jdouble x){std::lock_guard<std::mutex>l(mutex_);value_=function_->applyAsDouble(value_,x);}::jxx::lang::jdouble DoubleAccumulator::get()const{std::lock_guard<std::mutex>l(mutex_);return value_;}void DoubleAccumulator::reset(){std::lock_guard<std::mutex>l(mutex_);value_=identity_;}::jxx::lang::jdouble DoubleAccumulator::getThenReset(){std::lock_guard<std::mutex>l(mutex_);auto v=value_;value_=identity_;return v;}
::jxx::lang::jint DoubleAccumulator::intValue()const{return static_cast<::jxx::lang::jint>(get());}::jxx::lang::jlong DoubleAccumulator::longValue()const{return static_cast<::jxx::lang::jlong>(get());}::jxx::lang::jfloat DoubleAccumulator::floatValue()const{return static_cast<::jxx::lang::jfloat>(get());}::jxx::lang::jdouble DoubleAccumulator::doubleValue()const{return static_cast<::jxx::lang::jdouble>(get());}::jxx::Ptr<::jxx::lang::String>DoubleAccumulator::toString()const{return ::jxx::NEW<::jxx::lang::String>(std::to_string(get()));}void DoubleAccumulator::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&o){(void)o;}void DoubleAccumulator::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&i){(void)i;}void DoubleAccumulator::readObjectNoData(){}
}
