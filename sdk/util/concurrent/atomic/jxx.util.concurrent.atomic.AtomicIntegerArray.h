#pragma once
#include <mutex>
#include <vector>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::util::concurrent::atomic {
class AtomicIntegerArray final : public ::jxx::lang::ClassBase<AtomicIntegerArray,::jxx::lang::Object,::jxx::io::SerializableI> {
public:
 using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<AtomicIntegerArray,JxxSuper,::jxx::io::SerializableI>;using JxxClassInfoMarker=::jxx::lang::ClassInfo<AtomicIntegerArray,JxxSuper,::jxx::io::SerializableI>;static ::jxx::Ptr<::jxx::lang::ClassAny> Class(){return JxxClassInfoMarker::Class();}
 explicit AtomicIntegerArray(::jxx::lang::jint length):Super(),values_(checkedLength_(length)){}
 explicit AtomicIntegerArray(const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint,1U>>& array):Super(){if(!array)throw ::jxx::lang::NullPointerException();values_.reserve(array->length);for(std::uint32_t i=0;i<array->length;++i)values_.push_back((*array)[i]);}
 ::jxx::lang::jint length()const noexcept{return static_cast<::jxx::lang::jint>(values_.size());}
 ::jxx::lang::jint get(::jxx::lang::jint i)const{std::lock_guard<std::mutex>l(mutex_);return values_[index_(i)];}void set(::jxx::lang::jint i,::jxx::lang::jint v){std::lock_guard<std::mutex>l(mutex_);values_[index_(i)]=v;}void lazySet(::jxx::lang::jint i,::jxx::lang::jint v){set(i,v);}
 ::jxx::lang::jint getAndSet(::jxx::lang::jint i,::jxx::lang::jint v){std::lock_guard<std::mutex>l(mutex_);auto&x=values_[index_(i)];auto old=x;x=v;return old;}::jxx::lang::jbool compareAndSet(::jxx::lang::jint i,::jxx::lang::jint e,::jxx::lang::jint u){std::lock_guard<std::mutex>l(mutex_);auto&x=values_[index_(i)];if(x!=e)return false;x=u;return true;}::jxx::lang::jbool weakCompareAndSet(::jxx::lang::jint i,::jxx::lang::jint e,::jxx::lang::jint u){return compareAndSet(i,e,u);}
 ::jxx::lang::jint getAndIncrement(::jxx::lang::jint i){return getAndAdd(i,1);}::jxx::lang::jint getAndDecrement(::jxx::lang::jint i){return getAndAdd(i,-1);}::jxx::lang::jint getAndAdd(::jxx::lang::jint i,::jxx::lang::jint d){std::lock_guard<std::mutex>l(mutex_);auto&x=values_[index_(i)];auto old=x;x+=d;return old;}::jxx::lang::jint incrementAndGet(::jxx::lang::jint i){return addAndGet(i,1);}::jxx::lang::jint decrementAndGet(::jxx::lang::jint i){return addAndGet(i,-1);}::jxx::lang::jint addAndGet(::jxx::lang::jint i,::jxx::lang::jint d){std::lock_guard<std::mutex>l(mutex_);return values_[index_(i)]+=d;}
 void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&o)override{(void)o;}void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&i)override{(void)i;}void readObjectNoData()override{}
private:static std::size_t checkedLength_(::jxx::lang::jint n){if(n<0)throw ::jxx::lang::IllegalArgumentException();return static_cast<std::size_t>(n);}std::size_t index_(::jxx::lang::jint i)const{if(i<0||i>=length())throw ::jxx::lang::IndexOutOfBoundsException();return static_cast<std::size_t>(i);}mutable std::mutex mutex_;std::vector<::jxx::lang::jint>values_;
};}
