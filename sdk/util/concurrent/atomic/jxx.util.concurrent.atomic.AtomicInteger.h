#pragma once
#include <atomic>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Number.h"
#include "lang/jxx.lang.String.h"
namespace jxx::util::concurrent::atomic {
class AtomicInteger final : public ::jxx::lang::ClassBase<AtomicInteger,::jxx::lang::Number> {
public:
 using JxxSuper=::jxx::lang::Number; using Super=::jxx::lang::ClassBase<AtomicInteger,JxxSuper>; using JxxClassInfoMarker=::jxx::lang::ClassInfo<AtomicInteger,JxxSuper>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class(){return JxxClassInfoMarker::Class();}
 AtomicInteger():Super(),value_(0){} explicit AtomicInteger(::jxx::lang::jint initial):Super(),value_(initial){}
 ::jxx::lang::jint get()const noexcept{return value_.load(std::memory_order_acquire);} void set(::jxx::lang::jint v)noexcept{value_.store(v,std::memory_order_release);} void lazySet(::jxx::lang::jint v)noexcept{value_.store(v,std::memory_order_relaxed);} ::jxx::lang::jint getAndSet(::jxx::lang::jint v)noexcept{return value_.exchange(v);}
 ::jxx::lang::jbool compareAndSet(::jxx::lang::jint e,::jxx::lang::jint u)noexcept{return value_.compare_exchange_strong(e,u);} ::jxx::lang::jbool weakCompareAndSet(::jxx::lang::jint e,::jxx::lang::jint u)noexcept{return value_.compare_exchange_weak(e,u);}
 ::jxx::lang::jint getAndIncrement()noexcept{return value_.fetch_add(1);} ::jxx::lang::jint getAndDecrement()noexcept{return value_.fetch_sub(1);} ::jxx::lang::jint getAndAdd(::jxx::lang::jint d)noexcept{return value_.fetch_add(d);} ::jxx::lang::jint incrementAndGet()noexcept{return value_.fetch_add(1)+1;} ::jxx::lang::jint decrementAndGet()noexcept{return value_.fetch_sub(1)-1;} ::jxx::lang::jint addAndGet(::jxx::lang::jint d)noexcept{return value_.fetch_add(d)+d;}
 ::jxx::lang::jint intValue()const override{return get();} ::jxx::lang::jlong longValue()const override{return get();} ::jxx::lang::jfloat floatValue()const override{return static_cast<::jxx::lang::jfloat>(get());} ::jxx::lang::jdouble doubleValue()const override{return static_cast<::jxx::lang::jdouble>(get());} ::jxx::Ptr<::jxx::lang::String> toString()const override{return ::jxx::NEW<::jxx::lang::String>(std::to_string(get()));}
private: std::atomic<::jxx::lang::jint> value_;
}; }
