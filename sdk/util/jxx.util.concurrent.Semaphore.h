#pragma once
#include <condition_variable>
#include <mutex>
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
namespace jxx::util::concurrent
{
	class Semaphore :public jxx::lang::Object
	{
	mutable std::mutex mutex_; std::condition_variable condition_; jxx::lang::jint permits_; jxx::lang::jbool fair_; public:explicit Semaphore(jxx::lang::jint permits) :Semaphore(permits, false)
	{
	}Semaphore(jxx::lang::jint permits, jxx::lang::jbool fair) :permits_(permits), fair_(fair)
	{
	}void acquire()
	{
		acquire(1);
	}void acquire(jxx::lang::jint permits)
	{
		if (permits < 0)throw jxx::lang::IllegalArgumentException(); std::unique_lock<std::mutex> l(mutex_); condition_.wait(l, [&]
	   {
				  return permits_ >= permits;
	   }); permits_ -= permits;
	}jxx::lang::jbool tryAcquire()
	{
		return tryAcquire(1);
	}jxx::lang::jbool tryAcquire(jxx::lang::jint permits)
	{
		if (permits < 0)throw jxx::lang::IllegalArgumentException(); std::lock_guard<std::mutex> l(mutex_); if (permits_ < permits)return false; permits_ -= permits; return true;
	}jxx::lang::jbool tryAcquire(jxx::lang::jint permits, jxx::lang::jlong timeout, const jxx::Ptr<TimeUnit>& unit)
	{
		if (unit == nullptr)throw jxx::lang::NullPointerException(); if (permits < 0)throw jxx::lang::IllegalArgumentException(); std::unique_lock<std::mutex> l(mutex_); if (!condition_.wait_for(l, unit->toChrono(timeout), [&]
			{
				return permits_ >= permits;
			}))return false; permits_ -= permits; return true;
	}void release()
	{
		release(1);
	}void release(jxx::lang::jint permits)
	{
		if (permits < 0)throw jxx::lang::IllegalArgumentException();
		{
			std::lock_guard<std::mutex> l(mutex_); permits_ += permits;
		}condition_.notify_all();
	}jxx::lang::jint availablePermits() const
	{
		std::lock_guard<std::mutex> l(mutex_); return permits_;
	}jxx::lang::jbool isFair() const noexcept
	{
		return fair_;
	}protected:jxx::Ptr<jxx::lang::Object> cloneImpl() const override
	{
		throw jxx::lang::CloneNotSupportedException();
	}
	};
}
