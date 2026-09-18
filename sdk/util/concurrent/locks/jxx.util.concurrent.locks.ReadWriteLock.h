#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.Lock.h"
namespace jxx::util::concurrent::locks {
class ReadWriteLock : public ::jxx::lang::InterfaceBase<ReadWriteLock> {
public:
 using Super=::jxx::lang::InterfaceBase<ReadWriteLock>;using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;
 ~ReadWriteLock() override=default;
 virtual ::jxx::Ptr<Lock> readLock()=0;
 virtual ::jxx::Ptr<Lock> writeLock()=0;
}; }
