#pragma once
#include <mutex>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Number.h"
#include "lang/jxx.lang.String.h"
#include "util/function/jxx.util.function.LongBinaryOperator.h"
namespace jxx::util::concurrent::atomic {
class LongAccumulator final:public ::jxx::lang::ClassBase<LongAccumulator,::jxx::lang::Number,::jxx::io::SerializableI>{
public:using JxxSuper=::jxx::lang::Number;using Super=::jxx::lang::ClassBase<LongAccumulator,JxxSuper,::jxx::io::SerializableI>;using JxxClassInfoMarker=::jxx::lang::ClassInfo<LongAccumulator,JxxSuper,::jxx::io::SerializableI>;static ::jxx::Ptr<::jxx::lang::ClassAny>Class(){return JxxClassInfoMarker::Class();}
 LongAccumulator(const ::jxx::Ptr<::jxx::util::function::LongBinaryOperator>& function,::jxx::lang::jlong identity):Super(),function_(function),identity_(identity),value_(identity){if(!function_)throw ::jxx::lang::NullPointerException();}
 void accumulate(::jxx::lang::jlong x){std::lock_guard<std::mutex>l(mutex_);value_=function_->applyAsLong(value_,x);}::jxx::lang::jlong get()const{std::lock_guard<std::mutex>l(mutex_);return value_;}void reset(){std::lock_guard<std::mutex>l(mutex_);value_=identity_;}::jxx::lang::jlong getThenReset(){std::lock_guard<std::mutex>l(mutex_);auto v=value_;value_=identity_;return v;}
 ::jxx::lang::jint intValue()const override{return static_cast<::jxx::lang::jint>(get());}::jxx::lang::jlong longValue()const override{return get();}::jxx::lang::jfloat floatValue()const override{return static_cast<::jxx::lang::jfloat>(get());}::jxx::lang::jdouble doubleValue()const override{return static_cast<::jxx::lang::jdouble>(get());}::jxx::Ptr<::jxx::lang::String>toString()const override{return ::jxx::NEW<::jxx::lang::String>(std::to_string(get()));}
 void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&o)override{(void)o;}void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&i)override{(void)i;}void readObjectNoData()override{}
private:mutable std::mutex mutex_;::jxx::Ptr<::jxx::util::function::LongBinaryOperator>function_;::jxx::lang::jlong identity_;::jxx::lang::jlong value_;};}
