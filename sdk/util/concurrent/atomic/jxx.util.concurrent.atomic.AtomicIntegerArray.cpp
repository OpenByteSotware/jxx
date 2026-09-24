#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicIntegerArray.h"

#include "lang/jxx.lang.Exceptions.h"
namespace jxx::util::concurrent::atomic {
::jxx::Ptr<::jxx::lang::ClassAny> AtomicIntegerArray::Class(){return JxxClassInfoMarker::Class();}
AtomicIntegerArray::AtomicIntegerArray(::jxx::lang::jint n):Super(),values_(checkedLength_(n)){}
AtomicIntegerArray::AtomicIntegerArray(const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint,1U>>& a):Super(){if(!a)throw ::jxx::lang::NullPointerException();values_.reserve(a->length);for(::jxx::lang::jint i=0;i<a->length;++i)values_.push_back((*a)[i]);}
::jxx::lang::jint AtomicIntegerArray::length()const noexcept{return static_cast<::jxx::lang::jint>(values_.size());}
::jxx::lang::jint AtomicIntegerArray::get(::jxx::lang::jint i)const{std::lock_guard<std::mutex>l(mutex_);return values_[index_(i)];}
void AtomicIntegerArray::set(::jxx::lang::jint i,::jxx::lang::jint v){std::lock_guard<std::mutex>l(mutex_);values_[index_(i)]=v;}
void AtomicIntegerArray::lazySet(::jxx::lang::jint i,::jxx::lang::jint v){set(i,v);}
::jxx::lang::jint AtomicIntegerArray::getAndSet(::jxx::lang::jint i,::jxx::lang::jint v){std::lock_guard<std::mutex>l(mutex_);auto&x=values_[index_(i)];auto old=x;x=v;return old;}
::jxx::lang::jbool AtomicIntegerArray::compareAndSet(::jxx::lang::jint i,::jxx::lang::jint e,::jxx::lang::jint u){std::lock_guard<std::mutex>l(mutex_);auto&x=values_[index_(i)];if(x!=e)return false;x=u;return true;}
::jxx::lang::jbool AtomicIntegerArray::weakCompareAndSet(::jxx::lang::jint i,::jxx::lang::jint e,::jxx::lang::jint u){return compareAndSet(i,e,u);}
::jxx::lang::jint AtomicIntegerArray::getAndIncrement(::jxx::lang::jint i){return getAndAdd(i,1);} ::jxx::lang::jint AtomicIntegerArray::getAndDecrement(::jxx::lang::jint i){return getAndAdd(i,-1);}
::jxx::lang::jint AtomicIntegerArray::getAndAdd(::jxx::lang::jint i,::jxx::lang::jint d){std::lock_guard<std::mutex>l(mutex_);auto&x=values_[index_(i)];auto old=x;x+=d;return old;}
::jxx::lang::jint AtomicIntegerArray::incrementAndGet(::jxx::lang::jint i){return addAndGet(i,1);} ::jxx::lang::jint AtomicIntegerArray::decrementAndGet(::jxx::lang::jint i){return addAndGet(i,-1);}
::jxx::lang::jint AtomicIntegerArray::addAndGet(::jxx::lang::jint i,::jxx::lang::jint d){std::lock_guard<std::mutex>l(mutex_);return values_[index_(i)]+=d;}
void AtomicIntegerArray::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&o){(void)o;}void AtomicIntegerArray::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&i){(void)i;}void AtomicIntegerArray::readObjectNoData(){}
std::size_t AtomicIntegerArray::checkedLength_(::jxx::lang::jint n){if(n<0)throw ::jxx::lang::IllegalArgumentException();return static_cast<std::size_t>(n);}
std::size_t AtomicIntegerArray::index_(::jxx::lang::jint i)const{if(i<0||i>=length())throw ::jxx::lang::IndexOutOfBoundsException();return static_cast<std::size_t>(i);}
} // namespace jxx::util::concurrent::atomic
