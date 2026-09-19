#pragma once
#include <mutex>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Number.h"
#include "lang/jxx.lang.String.h"
#include "util/function/jxx.util.function.DoubleBinaryOperator.h"
namespace jxx::util::concurrent::atomic {
class DoubleAccumulator final:public ::jxx::lang::ClassBase<DoubleAccumulator,::jxx::lang::Number,::jxx::io::SerializableI>{
public:using JxxSuper=::jxx::lang::Number;using Super=::jxx::lang::ClassBase<DoubleAccumulator,JxxSuper,::jxx::io::SerializableI>;using JxxClassInfoMarker=::jxx::lang::ClassInfo<DoubleAccumulator,JxxSuper,::jxx::io::SerializableI>;static ::jxx::Ptr<::jxx::lang::ClassAny>Class(){return JxxClassInfoMarker::Class();}
 DoubleAccumulator(const ::jxx::Ptr<::jxx::util::function::DoubleBinaryOperator>& function,::jxx::lang::jdouble identity):Super(),function_(function),identity_(identity),value_(identity){if(!function_)throw ::jxx::lang::NullPointerException();}
 void accumulate(::jxx::lang::jdouble x){std::lock_guard<std::mutex>l(mutex_);value_=function_->applyAsDouble(value_,x);}::jxx::lang::jdouble get()const{std::lock_guard<std::mutex>l(mutex_);return value_;}void reset(){std::lock_guard<std::mutex>l(mutex_);value_=identity_;}::jxx::lang::jdouble getThenReset(){std::lock_guard<std::mutex>l(mutex_);auto v=value_;value_=identity_;return v;}
 ::jxx::lang::jint intValue()const override{return static_cast<::jxx::lang::jint>(get());}::jxx::lang::jlong longValue()const override{return static_cast<::jxx::lang::jlong>(get());}::jxx::lang::jfloat floatValue()const override{return static_cast<::jxx::lang::jfloat>(get());}::jxx::lang::jdouble doubleValue()const override{return get();}::jxx::Ptr<::jxx::lang::String>toString()const override{return ::jxx::NEW<::jxx::lang::String>(std::to_string(get()));}
 void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&o)override{(void)o;}void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&i)override{(void)i;}void readObjectNoData()override{}
private:mutable std::mutex mutex_;::jxx::Ptr<::jxx::util::function::DoubleBinaryOperator>function_;::jxx::lang::jdouble identity_;::jxx::lang::jdouble value_;};}
