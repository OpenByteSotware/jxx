#include "util/concurrent/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/concurrent/jxx.util.concurrent.RejectedExecutionException.h"
namespace jxx::util::concurrent
{
	::jxx::Ptr<::jxx::lang::ClassAny> ScheduledThreadPoolExecutor::Class()
	{
		return JxxClassInfoMarker::Class();
	}
	ScheduledThreadPoolExecutor::ScheduledThreadPoolExecutor(::jxx::lang::jint core) :Super(core, core, 0, TimeUnit::NANOSECONDS())
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
	::jxx::Ptr<ScheduledFuture<::jxx::lang::Object>> ScheduledThreadPoolExecutor::schedule(const ::jxx::Ptr<::jxx::lang::Runnable>& c, ::jxx::lang::jlong d, const ::jxx::Ptr<TimeUnit>& u)
	{
		if (!u)throw ::jxx::lang::NullPointerException(); return schedule_(c, std::max<::jxx::lang::jlong>(0, u->toNanos(d)), 0);
	}
	::jxx::Ptr<ScheduledFuture<::jxx::lang::Object>> ScheduledThreadPoolExecutor::scheduleAtFixedRate(const ::jxx::Ptr<::jxx::lang::Runnable>& c, ::jxx::lang::jlong d, ::jxx::lang::jlong p, const ::jxx::Ptr<TimeUnit>& u)
	{
		if (!u)throw ::jxx::lang::NullPointerException(); if (p <= 0)throw ::jxx::lang::IllegalArgumentException(); return schedule_(c, std::max<::jxx::lang::jlong>(0, u->toNanos(d)), u->toNanos(p));
	}
	::jxx::Ptr<ScheduledFuture<::jxx::lang::Object>> ScheduledThreadPoolExecutor::scheduleWithFixedDelay(const ::jxx::Ptr<::jxx::lang::Runnable>& c, ::jxx::lang::jlong d, ::jxx::lang::jlong p, const ::jxx::Ptr<TimeUnit>& u)
	{
		if (!u)throw ::jxx::lang::NullPointerException(); if (p <= 0)throw ::jxx::lang::IllegalArgumentException(); return schedule_(c, std::max<::jxx::lang::jlong>(0, u->toNanos(d)), -u->toNanos(p));
	}
	::jxx::Ptr<ScheduledFuture<::jxx::lang::Object>> ScheduledThreadPoolExecutor::schedule_(const ::jxx::Ptr<::jxx::lang::Runnable>& command, ::jxx::lang::jlong delayNanos, ::jxx::lang::jlong periodNanos)
	{
		if (command == nullptr) throw ::jxx::lang::NullPointerException();
		const auto task = ::jxx::NEW<Task>(command, nullptr, delayNanos, periodNanos);
		{
			std::lock_guard<std::mutex> lock(scheduleMutex_);
			if (shutdownRequested_.load(std::memory_order_acquire) || stopping_) throw RejectedExecutionException();
			scheduled_.push(task);
		}
		scheduleChanged_.notify_all();
		return ::jxx::CAST<ScheduledFuture<::jxx::lang::Object>>(task);
	}
	void ScheduledThreadPoolExecutor::execute(const ::jxx::Ptr<::jxx::lang::Runnable>& c)
	{
		(void)schedule(c, 0, TimeUnit::NANOSECONDS());
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
			ThreadPoolExecutor::execute(
				::jxx::CAST<::jxx::lang::Runnable>(runner));
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
		shutdownRequested_.store(true, std::memory_order_release);
		{
			std::lock_guard<std::mutex>l(scheduleMutex_); stopping_ = true; while (!scheduled_.empty()) {
				scheduled_.top()->cancel(false); scheduled_.pop();
			}
		}
		scheduleChanged_.notify_all(); return ThreadPoolExecutor::shutdownNow();
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
