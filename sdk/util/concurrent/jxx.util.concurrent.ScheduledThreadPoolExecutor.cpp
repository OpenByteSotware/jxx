#include "util/concurrent/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFutureAnyAdapter.h"
#include "util/jxx.util.ArrayList.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/concurrent/jxx.util.concurrent.RejectedExecutionException.h"
namespace jxx::util::concurrent
{
	::jxx::Ptr<::jxx::lang::ClassAny> ScheduledThreadPoolExecutor::Class()
	{
		return JxxClassInfoMarker::Class();
	}
	ScheduledThreadPoolExecutor::ScheduledThreadPoolExecutor(::jxx::lang::jint core)
        : ScheduledThreadPoolExecutor(core, Executors::defaultThreadFactory()) {}
	ScheduledThreadPoolExecutor::ScheduledThreadPoolExecutor(
        ::jxx::lang::jint core,
        const ::jxx::Ptr<ThreadFactory>& threadFactory)
        : Super(core, std::max<::jxx::lang::jint>(1, core), 0,
              TimeUnit::NANOSECONDS(), threadFactory)
	{
		dispatcher_ = std::thread([this]
	   {
				  dispatch_();
	   });
	}
	ScheduledThreadPoolExecutor::~ScheduledThreadPoolExecutor()
	{
		shutdown();
		if (dispatcher_.joinable()) dispatcher_.join();
		joinWorkers_();
	}
	::jxx::Ptr<ScheduledFuture<::jxx::lang::Object>> ScheduledThreadPoolExecutor::schedule(
        const ::jxx::Ptr<::jxx::lang::Runnable>& command,
        ::jxx::lang::jlong delay,
        const ::jxx::Ptr<TimeUnit>& unit)
	{
        if (command == nullptr || unit == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        return schedule_(command,
            std::max<::jxx::lang::jlong>(0, unit->toNanos(delay)),
            0,
            ConcreteTask::PeriodicMode::OneShot);
	}

	::jxx::Ptr<ScheduledFuture<::jxx::lang::Object>> ScheduledThreadPoolExecutor::scheduleAtFixedRate(
        const ::jxx::Ptr<::jxx::lang::Runnable>& command,
        ::jxx::lang::jlong initialDelay,
        ::jxx::lang::jlong period,
        const ::jxx::Ptr<TimeUnit>& unit)
	{
        if (command == nullptr || unit == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        if (period <= 0) throw ::jxx::lang::IllegalArgumentException();
        return schedule_(command,
            std::max<::jxx::lang::jlong>(0, unit->toNanos(initialDelay)),
            unit->toNanos(period),
            ConcreteTask::PeriodicMode::FixedRate);
	}

	::jxx::Ptr<ScheduledFuture<::jxx::lang::Object>> ScheduledThreadPoolExecutor::scheduleWithFixedDelay(
        const ::jxx::Ptr<::jxx::lang::Runnable>& command,
        ::jxx::lang::jlong initialDelay,
        ::jxx::lang::jlong delay,
        const ::jxx::Ptr<TimeUnit>& unit)
	{
        if (command == nullptr || unit == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        if (delay <= 0) throw ::jxx::lang::IllegalArgumentException();
        return schedule_(command,
            std::max<::jxx::lang::jlong>(0, unit->toNanos(initialDelay)),
            unit->toNanos(delay),
            ConcreteTask::PeriodicMode::FixedDelay);
	}
	::jxx::Ptr<ScheduledFuture<::jxx::lang::Object>> ScheduledThreadPoolExecutor::schedule_(const ::jxx::Ptr<::jxx::lang::Runnable>& command, ::jxx::lang::jlong delayNanos, ::jxx::lang::jlong periodNanos,
        ConcreteTask::PeriodicMode periodicMode)
	{
		if (command == nullptr) throw ::jxx::lang::NullPointerException();
		const auto concreteTask = ::jxx::NEW<ScheduledFutureTask<::jxx::lang::Object>>(
            command, nullptr, delayNanos, periodNanos, periodicMode);
		const auto task = ::jxx::CAST<Task>(concreteTask);
		{
			std::lock_guard<std::mutex> lock(scheduleMutex_);
			if (shutdownRequested_.load(std::memory_order_acquire) || stopping_) throw RejectedExecutionException();
			scheduled_.push(task);
		}
		scheduleChanged_.notify_all();
		return ::jxx::CAST<ScheduledFuture<::jxx::lang::Object>>(concreteTask);
	}
	void ScheduledThreadPoolExecutor::enqueueScheduledTask_(
        const ::jxx::Ptr<scheduled_detail::ScheduledTask>& task)
    {
        if (task == nullptr) throw ::jxx::lang::NullPointerException();
        {
            std::lock_guard<std::mutex> lock(scheduleMutex_);
            if (shutdownRequested_.load(std::memory_order_acquire) || stopping_) {
                throw RejectedExecutionException();
            }
            scheduled_.push(task);
        }
        scheduleChanged_.notify_all();
    }

	void ScheduledThreadPoolExecutor::execute(const ::jxx::Ptr<::jxx::lang::Runnable>& c)
	{
		(void)schedule(c, 0, TimeUnit::NANOSECONDS());
	}
	::jxx::Ptr<wildcard::FutureAny> ScheduledThreadPoolExecutor::submit(
        const ::jxx::Ptr<::jxx::lang::Runnable>& task)
    {
        const auto scheduled = schedule(
            task, 0, TimeUnit::NANOSECONDS());
        return ::jxx::CAST<wildcard::FutureAny>(
            ::jxx::NEW<scheduled_detail::ScheduledFutureAnyAdapter>(
                scheduled));
    }
	void ScheduledThreadPoolExecutor::dispatch_()
	{
		for (;;) {
			::jxx::Ptr<Task> task;
			{
				std::unique_lock<std::mutex> lock(scheduleMutex_);
				for (;;) {
					if (stopping_ && scheduled_.empty()) return;
					if (scheduled_.empty()) {
						scheduleChanged_.wait(lock);
						continue;
					}
					task = scheduled_.top();
					const auto delay = task->getDelay(TimeUnit::NANOSECONDS());
					if (delay > 0) {
						scheduleChanged_.wait_for(
							lock, std::chrono::nanoseconds(delay));
						continue;
					}
					scheduled_.pop();
					break;
				}
			}
			if (task->isCancelled()) {
				afterExecute_(task);
				continue;
			}
			const auto runner = ::jxx::NEW<ScheduledTaskRunner>(this, task);
            try {
                ThreadPoolExecutor::execute(
                    ::jxx::CAST<::jxx::lang::Runnable>(runner));
            } catch (const RejectedExecutionException&) {
                // The scheduler already accepted this task, but the backing
                // executor could not create or retain a worker. Do not allow
                // the dispatcher thread to terminate with an uncaught
                // exception or leave the returned future incomplete.
                (void)task->cancel(false);
                afterExecute_(task);
            }
		}
	}

	void ScheduledThreadPoolExecutor::afterExecute_(
		const ::jxx::Ptr<Task>& task)
	{
		::jxx::lang::jbool stopBase = false;
		if (task != nullptr && task->isPeriodic() &&
			!task->isCancelled() && !task->isDone()) {
			::jxx::lang::jbool keep = false;
			{
				std::lock_guard<std::mutex> lock(scheduleMutex_);
				keep = (!shutdownRequested_.load(std::memory_order_acquire) || continuePeriodicAfterShutdown_) && !stopping_;
				if (keep) scheduled_.push(task);
			}
			if (!keep) (void)task->cancel(false);
		}

		{
			std::lock_guard<std::mutex> lock(scheduleMutex_);
			if (shutdownRequested_.load(std::memory_order_acquire) &&
				scheduled_.empty()) {
				stopping_ = true;
				stopBase = true;
			}
		}
		scheduleChanged_.notify_all();
		if (stopBase) ThreadPoolExecutor::shutdown();
	}
	void ScheduledThreadPoolExecutor::shutdown()
	{
		shutdownRequested_.store(true, std::memory_order_release);
		{
			std::lock_guard<std::mutex> l(scheduleMutex_);
			std::vector<::jxx::Ptr<Task>> retained;
			while (!scheduled_.empty()) {
				auto task = scheduled_.top(); scheduled_.pop();
				const auto keep = task->isPeriodic()
					? continuePeriodicAfterShutdown_
					: executeDelayedAfterShutdown_;
				if (keep) retained.push_back(task); else task->cancel(false);
			}
			for (const auto& task : retained) scheduled_.push(task);
			stopping_ = scheduled_.empty();
		}
		scheduleChanged_.notify_all();
		if (stopping_) ThreadPoolExecutor::shutdown();
	}

	::jxx::Ptr<::jxx::util::List<::jxx::lang::Runnable>> ScheduledThreadPoolExecutor::shutdownNow()
	{
        auto pending = ::jxx::NEW<::jxx::util::ArrayList<::jxx::lang::Runnable>>();
		shutdownRequested_.store(true, std::memory_order_release);
		{
			std::lock_guard<std::mutex> lock(scheduleMutex_);
            stopping_ = true;
            while (!scheduled_.empty()) {
                const auto task = scheduled_.top();
                scheduled_.pop();
                (void)task->cancel(false);
                pending->add(::jxx::CAST<::jxx::lang::Runnable>(task));
            }
		}
		scheduleChanged_.notify_all();
        const auto basePending = ThreadPoolExecutor::shutdownNow();
        if (basePending != nullptr) {
            for (::jxx::lang::jint index = 0; index < basePending->size(); ++index) {
                pending->add(basePending->get(index));
            }
        }
        return ::jxx::CAST<::jxx::util::List<::jxx::lang::Runnable>>(pending);
	}

	::jxx::lang::jbool ScheduledThreadPoolExecutor::isShutdown()
	{
		return shutdownRequested_.load(std::memory_order_acquire) || ThreadPoolExecutor::isShutdown();
	}

	void ScheduledThreadPoolExecutor::setContinueExistingPeriodicTasksAfterShutdownPolicy(::jxx::lang::jbool value)
	{
		::jxx::lang::jbool stopBase = false;
		{
			std::lock_guard<std::mutex> lock(scheduleMutex_);
			continuePeriodicAfterShutdown_ = value;
			if (!value && shutdownRequested_.load(std::memory_order_acquire)) {
				std::vector<::jxx::Ptr<Task>> retained;
				while (!scheduled_.empty()) {
					const auto task = scheduled_.top(); scheduled_.pop(); if (task->isPeriodic()) (void)task->cancel(false); else retained.push_back(task);
				}
				for (const auto& task : retained) scheduled_.push(task);
				if (scheduled_.empty()) {
					stopping_ = true; stopBase = true;
				}
			}
		}
		scheduleChanged_.notify_all();
		if (stopBase) ThreadPoolExecutor::shutdown();
	}
	::jxx::lang::jbool ScheduledThreadPoolExecutor::getContinueExistingPeriodicTasksAfterShutdownPolicy() const
	{
		std::lock_guard<std::mutex> lock(scheduleMutex_); return continuePeriodicAfterShutdown_;
	}
	void ScheduledThreadPoolExecutor::setExecuteExistingDelayedTasksAfterShutdownPolicy(::jxx::lang::jbool value)
	{
		::jxx::lang::jbool stopBase = false;
		{
			std::lock_guard<std::mutex> lock(scheduleMutex_);
			executeDelayedAfterShutdown_ = value;
			if (!value && shutdownRequested_.load(std::memory_order_acquire)) {
				std::vector<::jxx::Ptr<Task>> retained;
				while (!scheduled_.empty()) {
					const auto task = scheduled_.top(); scheduled_.pop(); if (!task->isPeriodic()) (void)task->cancel(false); else retained.push_back(task);
				}
				for (const auto& task : retained) scheduled_.push(task);
				if (scheduled_.empty()) {
					stopping_ = true; stopBase = true;
				}
			}
		}
		scheduleChanged_.notify_all();
		if (stopBase) ThreadPoolExecutor::shutdown();
	}
	::jxx::lang::jbool ScheduledThreadPoolExecutor::getExecuteExistingDelayedTasksAfterShutdownPolicy() const
	{
		std::lock_guard<std::mutex> lock(scheduleMutex_); return executeDelayedAfterShutdown_;
	}
} // namespace jxx::util::concurrent
