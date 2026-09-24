#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicLongArray.h"

#include "lang/jxx.lang.Exceptions.h"
namespace jxx::util::concurrent::atomic {
::jxx::Ptr<::jxx::lang::ClassAny> AtomicLongArray::Class(){return JxxClassInfoMarker::Class();}
AtomicLongArray::AtomicLongArray(::jxx::lang::jint n):Super(),values_(checkedLength_(n)){}
AtomicLongArray::AtomicLongArray(const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jlong,1U>>& a):Super(){if(!a)throw ::jxx::lang::NullPointerException();values_.reserve(a->length);for(::jxx::lang::jint i=0;i<a->length;++i)values_.push_back((*a)[i]);}
::jxx::lang::jint AtomicLongArray::length()const noexcept{return static_cast<::jxx::lang::jint>(values_.size());}
::jxx::lang::jlong AtomicLongArray::get(::jxx::lang::jint i)const{std::lock_guard<std::mutex>l(mutex_);return values_[index_(i)];}
void AtomicLongArray::set(::jxx::lang::jint i,::jxx::lang::jlong v){std::lock_guard<std::mutex>l(mutex_);values_[index_(i)]=v;}
void AtomicLongArray::lazySet(::jxx::lang::jint i,::jxx::lang::jlong v){set(i,v);}
::jxx::lang::jlong AtomicLongArray::getAndSet(::jxx::lang::jint i,::jxx::lang::jlong v){std::lock_guard<std::mutex>l(mutex_);auto&x=values_[index_(i)];auto old=x;x=v;return old;}
::jxx::lang::jbool AtomicLongArray::compareAndSet(::jxx::lang::jint i,::jxx::lang::jlong e,::jxx::lang::jlong u){std::lock_guard<std::mutex>l(mutex_);auto&x=values_[index_(i)];if(x!=e)return false;x=u;return true;}
::jxx::lang::jbool AtomicLongArray::weakCompareAndSet(::jxx::lang::jint i,::jxx::lang::jlong e,::jxx::lang::jlong u){return compareAndSet(i,e,u);}
::jxx::lang::jlong AtomicLongArray::getAndIncrement(::jxx::lang::jint i){return getAndAdd(i,1);} ::jxx::lang::jlong AtomicLongArray::getAndDecrement(::jxx::lang::jint i){return getAndAdd(i,-1);}
::jxx::lang::jlong AtomicLongArray::getAndAdd(::jxx::lang::jint i,::jxx::lang::jlong d){std::lock_guard<std::mutex>l(mutex_);auto&x=values_[index_(i)];auto old=x;x+=d;return old;}
::jxx::lang::jlong AtomicLongArray::incrementAndGet(::jxx::lang::jint i){return addAndGet(i,1);} ::jxx::lang::jlong AtomicLongArray::decrementAndGet(::jxx::lang::jint i){return addAndGet(i,-1);}
::jxx::lang::jlong AtomicLongArray::addAndGet(::jxx::lang::jint i,::jxx::lang::jlong d){std::lock_guard<std::mutex>l(mutex_);return values_[index_(i)]+=d;}
void AtomicLongArray::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&o){(void)o;}void AtomicLongArray::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&i){(void)i;}void AtomicLongArray::readObjectNoData(){}
std::size_t AtomicLongArray::checkedLength_(::jxx::lang::jint n){if(n<0)throw ::jxx::lang::IllegalArgumentException();return static_cast<std::size_t>(n);}
std::size_t AtomicLongArray::index_(::jxx::lang::jint i)const{if(i<0||i>=length())throw ::jxx::lang::IndexOutOfBoundsException();return static_cast<std::size_t>(i);}
} // namespace jxx::util::concurrent::atomic
