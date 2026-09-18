#pragma once
#include <atomic>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Number.h"
#include "lang/jxx.lang.String.h"
namespace jxx::util::concurrent::atomic {
class AtomicLong final : public ::jxx::lang::ClassBase<AtomicLong,::jxx::lang::Number> {
public:
 using JxxSuper=::jxx::lang::Number; using Super=::jxx::lang::ClassBase<AtomicLong,JxxSuper>; using JxxClassInfoMarker=::jxx::lang::ClassInfo<AtomicLong,JxxSuper>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class(){return JxxClassInfoMarker::Class();}
 AtomicLong():Super(),value_(0){} explicit AtomicLong(::jxx::lang::jlong initial):Super(),value_(initial){}
 ::jxx::lang::jlong get()const noexcept{return value_.load(std::memory_order_acquire);} void set(::jxx::lang::jlong v)noexcept{value_.store(v,std::memory_order_release);} void lazySet(::jxx::lang::jlong v)noexcept{value_.store(v,std::memory_order_relaxed);} ::jxx::lang::jlong getAndSet(::jxx::lang::jlong v)noexcept{return value_.exchange(v);}
 ::jxx::lang::jbool compareAndSet(::jxx::lang::jlong e,::jxx::lang::jlong u)noexcept{return value_.compare_exchange_strong(e,u);} ::jxx::lang::jbool weakCompareAndSet(::jxx::lang::jlong e,::jxx::lang::jlong u)noexcept{return value_.compare_exchange_weak(e,u);}
 ::jxx::lang::jlong getAndIncrement()noexcept{return value_.fetch_add(1);} ::jxx::lang::jlong getAndDecrement()noexcept{return value_.fetch_sub(1);} ::jxx::lang::jlong getAndAdd(::jxx::lang::jlong d)noexcept{return value_.fetch_add(d);} ::jxx::lang::jlong incrementAndGet()noexcept{return value_.fetch_add(1)+1;} ::jxx::lang::jlong decrementAndGet()noexcept{return value_.fetch_sub(1)-1;} ::jxx::lang::jlong addAndGet(::jxx::lang::jlong d)noexcept{return value_.fetch_add(d)+d;}
 ::jxx::lang::jint intValue()const override{return get();} ::jxx::lang::jlong longValue()const override{return get();} ::jxx::lang::jfloat floatValue()const override{return static_cast<::jxx::lang::jfloat>(get());} ::jxx::lang::jdouble doubleValue()const override{return static_cast<::jxx::lang::jdouble>(get());} ::jxx::Ptr<::jxx::lang::String> toString()const override{return ::jxx::NEW<::jxx::lang::String>(std::to_string(get()));}
private: std::atomic<::jxx::lang::jlong> value_;
}; }
