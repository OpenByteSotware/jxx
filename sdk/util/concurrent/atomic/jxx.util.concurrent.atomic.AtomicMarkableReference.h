#pragma once
#include <mutex>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::util::concurrent::atomic {
template<typename V> class AtomicMarkableReference final : public ::jxx::lang::ClassBase<AtomicMarkableReference<V>,::jxx::lang::Object>{
public:using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<AtomicMarkableReference<V>,JxxSuper>;using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;static ::jxx::Ptr<::jxx::lang::ClassAny>Class(){return JxxClassInfoMarker::Class();}
 AtomicMarkableReference(const ::jxx::Ptr<V>& initialReference,::jxx::lang::jbool initialMark):Super(),reference_(initialReference),mark_(initialMark){}
 ::jxx::Ptr<V> getReference()const{std::lock_guard<std::mutex>l(mutex_);return reference_;}::jxx::lang::jbool isMarked()const{std::lock_guard<std::mutex>l(mutex_);return mark_;}
 ::jxx::Ptr<V> get(const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jbool,1U>>& holder)const{if(!holder||holder->length<1)throw ::jxx::lang::IllegalArgumentException();std::lock_guard<std::mutex>l(mutex_);(*holder)[0]=mark_;return reference_;}
 void set(const ::jxx::Ptr<V>& reference,::jxx::lang::jbool mark){std::lock_guard<std::mutex>l(mutex_);reference_=reference;mark_=mark;}
 ::jxx::lang::jbool compareAndSet(const ::jxx::Ptr<V>& expectedReference,const ::jxx::Ptr<V>& newReference,::jxx::lang::jbool expectedMark,::jxx::lang::jbool newMark){std::lock_guard<std::mutex>l(mutex_);if(reference_.get()!=expectedReference.get()||mark_!=expectedMark)return false;reference_=newReference;mark_=newMark;return true;}
 ::jxx::lang::jbool weakCompareAndSet(const ::jxx::Ptr<V>&e,const ::jxx::Ptr<V>&n,::jxx::lang::jbool em,::jxx::lang::jbool nm){return compareAndSet(e,n,em,nm);}::jxx::lang::jbool attemptMark(const ::jxx::Ptr<V>&e,::jxx::lang::jbool m){std::lock_guard<std::mutex>l(mutex_);if(reference_.get()!=e.get())return false;mark_=m;return true;}
private:mutable std::mutex mutex_;::jxx::Ptr<V>reference_;::jxx::lang::jbool mark_;};}
