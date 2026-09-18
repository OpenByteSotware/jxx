#pragma once
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx.lang.Object.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.Lock.h"
namespace jxx::util::concurrent::locks {
class ReentrantLock final : public ::jxx::lang::ClassBase<ReentrantLock,::jxx::lang::Object,Lock,::jxx::io::SerializableI> {
 class ConditionImpl final : public ::jxx::lang::ClassBase<ConditionImpl,::jxx::lang::Object,Condition> {
 public:
  using Super=::jxx::lang::ClassBase<ConditionImpl,::jxx::lang::Object,Condition>;
  explicit ConditionImpl(ReentrantLock* owner):Super(),owner_(owner){}
  void await() override{owner_->checkOwner_();std::unique_lock<std::recursive_timed_mutex> l(owner_->mutex_,std::adopt_lock);condition_.wait(l);l.release();}
  void awaitUninterruptibly() override{await();}
  ::jxx::lang::jlong awaitNanos(::jxx::lang::jlong nanos) override{owner_->checkOwner_();const auto start=std::chrono::steady_clock::now();std::unique_lock<std::recursive_timed_mutex>l(owner_->mutex_,std::adopt_lock);condition_.wait_for(l,std::chrono::nanoseconds(nanos));l.release();return nanos-std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-start).count();}
  ::jxx::lang::jbool await(::jxx::lang::jlong time,const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>&unit)override{if(!unit)throw ::jxx::lang::NullPointerException();owner_->checkOwner_();std::unique_lock<std::recursive_timed_mutex>l(owner_->mutex_,std::adopt_lock);auto result=condition_.wait_for(l,unit->toChrono(time))!=std::cv_status::timeout;l.release();return result;}
  void signal()override{owner_->checkOwner_();condition_.notify_one();} void signalAll()override{owner_->checkOwner_();condition_.notify_all();}
 private:ReentrantLock* owner_;std::condition_variable_any condition_;
 };
public:
 using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<ReentrantLock,JxxSuper,Lock,::jxx::io::SerializableI>;using JxxClassInfoMarker=::jxx::lang::ClassInfo<ReentrantLock,JxxSuper,Lock,::jxx::io::SerializableI>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class(){return JxxClassInfoMarker::Class();}
 ReentrantLock():ReentrantLock(false){} explicit ReentrantLock(::jxx::lang::jbool fair):Super(),fair_(fair){}
 void lock()override{mutex_.lock();acquired_();} void lockInterruptibly() override {
  if (::jxx::lang::Thread::interrupted()) {
   throw ::jxx::lang::InterruptedException();
  }
  while (!mutex_.try_lock_for(std::chrono::milliseconds(10))) {
   if (::jxx::lang::Thread::interrupted()) {
    throw ::jxx::lang::InterruptedException();
   }
  }
  acquired_();
 }
 ::jxx::lang::jbool tryLock()override{if(!mutex_.try_lock())return false;acquired_();return true;}
 ::jxx::lang::jbool tryLock(
  ::jxx::lang::jlong time,
  const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) override {
  if (!unit) throw ::jxx::lang::NullPointerException();
  if (::jxx::lang::Thread::interrupted()) {
   throw ::jxx::lang::InterruptedException();
  }
  const auto deadline = std::chrono::steady_clock::now() + unit->toChrono(time);
  for (;;) {
   const auto now = std::chrono::steady_clock::now();
   if (now >= deadline) return false;
   const auto remaining = deadline - now;
   if (mutex_.try_lock_for(std::min(
       remaining,
       std::chrono::steady_clock::duration(std::chrono::milliseconds(10))))) {
    acquired_();
    return true;
   }
   if (::jxx::lang::Thread::interrupted()) {
    throw ::jxx::lang::InterruptedException();
   }
  }
 }
 void unlock()override{checkOwner_();if(--holdCount_==0)owner_=std::thread::id{};mutex_.unlock();}
 ::jxx::Ptr<Condition> newCondition()override{return ::jxx::CAST<Condition>(::jxx::NEW<ConditionImpl>(this));}
 ::jxx::lang::jint getHoldCount()const noexcept{return owner_==std::this_thread::get_id()?holdCount_:0;} ::jxx::lang::jbool isHeldByCurrentThread()const noexcept{return owner_==std::this_thread::get_id();} ::jxx::lang::jbool isLocked()const noexcept{return holdCount_>0;} ::jxx::lang::jbool isFair()const noexcept{return fair_;}
 void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out)override{(void)out;}void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in)override{(void)in;}void readObjectNoData()override{}
private:
 void acquired_(){if(owner_==std::this_thread::get_id())++holdCount_;else{owner_=std::this_thread::get_id();holdCount_=1;}} void checkOwner_()const{if(owner_!=std::this_thread::get_id())throw ::jxx::lang::IllegalMonitorStateException();}
 mutable std::recursive_timed_mutex mutex_;std::thread::id owner_;::jxx::lang::jint holdCount_=0;::jxx::lang::jbool fair_;
}; }
