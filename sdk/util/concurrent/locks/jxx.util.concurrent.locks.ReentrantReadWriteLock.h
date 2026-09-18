#pragma once
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.ReadWriteLock.h"
namespace jxx::util::concurrent::locks {
class ReentrantReadWriteLock final : public ::jxx::lang::ClassBase<ReentrantReadWriteLock,::jxx::lang::Object,ReadWriteLock,::jxx::io::SerializableI> {
 class ReadLockImpl final:public ::jxx::lang::ClassBase<ReadLockImpl,::jxx::lang::Object,Lock>{public:using Super=::jxx::lang::ClassBase<ReadLockImpl,::jxx::lang::Object,Lock>;explicit ReadLockImpl(ReentrantReadWriteLock*o):Super(),o_(o){}void lock()override{o_->mutex_.lock_shared();}void lockInterruptibly()override{lock();}::jxx::lang::jbool tryLock()override{return o_->mutex_.try_lock_shared();}::jxx::lang::jbool tryLock(::jxx::lang::jlong t,const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>&u)override{if(!u)throw ::jxx::lang::NullPointerException();return o_->mutex_.try_lock_shared_for(u->toChrono(t));}void unlock()override{o_->mutex_.unlock_shared();}::jxx::Ptr<Condition>newCondition()override{throw ::jxx::lang::UnsupportedOperationException();}private:ReentrantReadWriteLock*o_;};
 class WriteLockImpl final:public ::jxx::lang::ClassBase<WriteLockImpl,::jxx::lang::Object,Lock>{public:using Super=::jxx::lang::ClassBase<WriteLockImpl,::jxx::lang::Object,Lock>;explicit WriteLockImpl(ReentrantReadWriteLock*o):Super(),o_(o){}void lock()override{o_->mutex_.lock();}void lockInterruptibly()override{lock();}::jxx::lang::jbool tryLock()override{return o_->mutex_.try_lock();}::jxx::lang::jbool tryLock(::jxx::lang::jlong t,const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>&u)override{if(!u)throw ::jxx::lang::NullPointerException();return o_->mutex_.try_lock_for(u->toChrono(t));}void unlock()override{o_->mutex_.unlock();}::jxx::Ptr<Condition>newCondition()override{throw ::jxx::lang::UnsupportedOperationException();}private:ReentrantReadWriteLock*o_;};
public:
 using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<ReentrantReadWriteLock,JxxSuper,ReadWriteLock,::jxx::io::SerializableI>;using JxxClassInfoMarker=::jxx::lang::ClassInfo<ReentrantReadWriteLock,JxxSuper,ReadWriteLock,::jxx::io::SerializableI>;static ::jxx::Ptr<::jxx::lang::ClassAny>Class(){return JxxClassInfoMarker::Class();}
 ReentrantReadWriteLock():ReentrantReadWriteLock(false){}explicit ReentrantReadWriteLock(::jxx::lang::jbool fair):Super(),fair_(fair),read_(::jxx::NEW<ReadLockImpl>(this)),write_(::jxx::NEW<WriteLockImpl>(this)){}
 ::jxx::Ptr<Lock>readLock()override{return ::jxx::CAST<Lock>(read_);}::jxx::Ptr<Lock>writeLock()override{return ::jxx::CAST<Lock>(write_);}::jxx::lang::jbool isFair()const noexcept{return fair_;}
 void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&out)override{(void)out;}void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&in)override{(void)in;}void readObjectNoData()override{}
private:std::shared_timed_mutex mutex_;::jxx::lang::jbool fair_;::jxx::Ptr<ReadLockImpl>read_;::jxx::Ptr<WriteLockImpl>write_;
}; }
