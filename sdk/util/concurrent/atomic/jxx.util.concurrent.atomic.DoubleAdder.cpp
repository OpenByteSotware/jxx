#include "util/concurrent/atomic/jxx.util.concurrent.atomic.DoubleAdder.h"
#include <string>
#include "lang/jxx.lang.String.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::util::concurrent::atomic {::jxx::Ptr<::jxx::lang::ClassAny>DoubleAdder::Class(){return JxxClassInfoMarker::Class();}DoubleAdder::DoubleAdder():Super(){}void DoubleAdder::add(::jxx::lang::jdouble x){std::lock_guard<std::mutex>l(mutex_);value_+=x;}::jxx::lang::jdouble DoubleAdder::sum()const{std::lock_guard<std::mutex>l(mutex_);return value_;}void DoubleAdder::reset(){std::lock_guard<std::mutex>l(mutex_);value_=0;}::jxx::lang::jdouble DoubleAdder::sumThenReset(){std::lock_guard<std::mutex>l(mutex_);auto v=value_;value_=0;return v;}::jxx::lang::jint DoubleAdder::intValue()const{return static_cast<::jxx::lang::jint>(sum());}::jxx::lang::jlong DoubleAdder::longValue()const{return static_cast<::jxx::lang::jlong>(sum());}::jxx::lang::jfloat DoubleAdder::floatValue()const{return static_cast<::jxx::lang::jfloat>(sum());}::jxx::lang::jdouble DoubleAdder::doubleValue()const{return sum();}::jxx::Ptr<::jxx::lang::String>DoubleAdder::toString()const{return ::jxx::NEW<::jxx::lang::String>(std::to_string(sum()));}void DoubleAdder::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) {
    if (output == nullptr) throw ::jxx::lang::NullPointerException();
    output->writeDouble(sum());
}
void DoubleAdder::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) {
    if (input == nullptr) throw ::jxx::lang::NullPointerException();
    const auto restored = input->readDouble();
    std::lock_guard<std::mutex> lock(mutex_);
    value_ = restored;
}
void DoubleAdder::readObjectNoData() {
    std::lock_guard<std::mutex> lock(mutex_);
    value_ = 0.0;
}}
