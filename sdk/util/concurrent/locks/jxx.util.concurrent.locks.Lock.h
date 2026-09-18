#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.Condition.h"
namespace jxx::util::concurrent::locks {
class Lock : public ::jxx::lang::InterfaceBase<Lock> {
public:
 using Super=::jxx::lang::InterfaceBase<Lock>;using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;
 ~Lock() override=default;
 virtual void lock()=0;
 virtual void lockInterruptibly()=0;
 virtual ::jxx::lang::jbool tryLock()=0;
 virtual ::jxx::lang::jbool tryLock(::jxx::lang::jlong time,const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit)=0;
 virtual void unlock()=0;
 virtual ::jxx::Ptr<Condition> newCondition()=0;
}; }
