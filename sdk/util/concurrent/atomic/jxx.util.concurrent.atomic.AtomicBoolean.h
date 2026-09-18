#pragma once
#include <atomic>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::util::concurrent::atomic {
class AtomicBoolean final : public ::jxx::lang::ClassBase<AtomicBoolean,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<AtomicBoolean,JxxSuper>; using JxxClassInfoMarker=::jxx::lang::ClassInfo<AtomicBoolean,JxxSuper>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class(){return JxxClassInfoMarker::Class();}
 AtomicBoolean():Super(),value_(false){} explicit AtomicBoolean(::jxx::lang::jbool initial):Super(),value_(initial){}
 ::jxx::lang::jbool get() const noexcept{return value_.load(std::memory_order_acquire);} void set(::jxx::lang::jbool value)noexcept{value_.store(value,std::memory_order_release);} void lazySet(::jxx::lang::jbool value)noexcept{value_.store(value,std::memory_order_relaxed);}
 ::jxx::lang::jbool getAndSet(::jxx::lang::jbool value)noexcept{return value_.exchange(value,std::memory_order_acq_rel);} ::jxx::lang::jbool compareAndSet(::jxx::lang::jbool expected,::jxx::lang::jbool update)noexcept{return value_.compare_exchange_strong(expected,update,std::memory_order_acq_rel);} ::jxx::lang::jbool weakCompareAndSet(::jxx::lang::jbool expected,::jxx::lang::jbool update)noexcept{return value_.compare_exchange_weak(expected,update,std::memory_order_acq_rel);}
 ::jxx::Ptr<::jxx::lang::String> toString()const override{return ::jxx::NEW<::jxx::lang::String>(get()?"true":"false");}
private: std::atomic<::jxx::lang::jbool> value_;
}; }
