#pragma once
#include <atomic>
#include <memory>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::util::concurrent::atomic {
template<typename V> class AtomicReference final : public ::jxx::lang::ClassBase<AtomicReference<V>,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<AtomicReference<V>,JxxSuper>;using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;static ::jxx::Ptr<::jxx::lang::ClassAny> Class(){return JxxClassInfoMarker::Class();}
 AtomicReference():Super(){} explicit AtomicReference(const ::jxx::Ptr<V>& initial):Super(),value_(initial){}
 ::jxx::Ptr<V> get()const noexcept{return std::atomic_load_explicit(&value_,std::memory_order_acquire);} void set(const ::jxx::Ptr<V>&v)noexcept{std::atomic_store_explicit(&value_,v,std::memory_order_release);} void lazySet(const ::jxx::Ptr<V>&v)noexcept{std::atomic_store_explicit(&value_,v,std::memory_order_relaxed);} ::jxx::Ptr<V> getAndSet(const ::jxx::Ptr<V>&v)noexcept{return std::atomic_exchange_explicit(&value_,v,std::memory_order_acq_rel);}
 ::jxx::lang::jbool compareAndSet(const ::jxx::Ptr<V>&expected,const ::jxx::Ptr<V>&update)noexcept{auto e=expected;return std::atomic_compare_exchange_strong_explicit(&value_,&e,update,std::memory_order_acq_rel,std::memory_order_acquire);} ::jxx::lang::jbool weakCompareAndSet(const ::jxx::Ptr<V>&expected,const ::jxx::Ptr<V>&update)noexcept{auto e=expected;return std::atomic_compare_exchange_weak_explicit(&value_,&e,update,std::memory_order_acq_rel,std::memory_order_acquire);}
 ::jxx::Ptr<::jxx::lang::String> toString()const override{auto v=get();return v? v->toString() : ::jxx::NEW<::jxx::lang::String>("null");}
private: mutable ::jxx::Ptr<V> value_;
}; }
