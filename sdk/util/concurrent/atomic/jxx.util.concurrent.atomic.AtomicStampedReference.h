#pragma once
#include <mutex>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::util::concurrent::atomic {
template<typename V> class AtomicStampedReference final : public ::jxx::lang::ClassBase<AtomicStampedReference<V>,::jxx::lang::Object>{
public:using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<AtomicStampedReference<V>,JxxSuper>;using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;static ::jxx::Ptr<::jxx::lang::ClassAny>Class(){return JxxClassInfoMarker::Class();}
 AtomicStampedReference(const ::jxx::Ptr<V>& initialReference,::jxx::lang::jint initialStamp):Super(),reference_(initialReference),stamp_(initialStamp){}
 ::jxx::Ptr<V> getReference()const{std::lock_guard<std::mutex>l(mutex_);return reference_;}::jxx::lang::jint getStamp()const{std::lock_guard<std::mutex>l(mutex_);return stamp_;}
 ::jxx::Ptr<V> get(const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint,1U>>& holder)const{if(!holder||holder->length<1)throw ::jxx::lang::IllegalArgumentException();std::lock_guard<std::mutex>l(mutex_);(*holder)[0]=stamp_;return reference_;}
 void set(const ::jxx::Ptr<V>& reference,::jxx::lang::jint stamp){std::lock_guard<std::mutex>l(mutex_);reference_=reference;stamp_=stamp;}
 ::jxx::lang::jbool compareAndSet(const ::jxx::Ptr<V>& expectedReference,const ::jxx::Ptr<V>& newReference,::jxx::lang::jint expectedStamp,::jxx::lang::jint newStamp){std::lock_guard<std::mutex>l(mutex_);if(reference_.get()!=expectedReference.get()||stamp_!=expectedStamp)return false;reference_=newReference;stamp_=newStamp;return true;}
 ::jxx::lang::jbool weakCompareAndSet(const ::jxx::Ptr<V>&e,const ::jxx::Ptr<V>&n,::jxx::lang::jint es,::jxx::lang::jint ns){return compareAndSet(e,n,es,ns);}::jxx::lang::jbool attemptStamp(const ::jxx::Ptr<V>&e,::jxx::lang::jint s){std::lock_guard<std::mutex>l(mutex_);if(reference_.get()!=e.get())return false;stamp_=s;return true;}
private:mutable std::mutex mutex_;::jxx::Ptr<V>reference_;::jxx::lang::jint stamp_;};}
