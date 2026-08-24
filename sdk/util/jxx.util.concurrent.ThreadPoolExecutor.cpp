#include "util/jxx.util.concurrent.ThreadPoolExecutor.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.ArrayList.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::util::concurrent
{
	ThreadPoolExecutor::ThreadPoolExecutor(jxx::lang::jint core, jxx::lang::jint maximum, jxx::lang::jlong keepAlive, const jxx::Ptr<TimeUnit>& unit)
		: corePoolSize_(core), maximumPoolSize_(maximum), keepAliveTime_(keepAlive), keepAliveUnit_(unit)
	{
		if (core < 0 || maximum <= 0 || maximum < core || keepAlive < 0) throw jxx::lang::IllegalArgumentException();
		if (unit == nullptr) throw jxx::lang::NullPointerException();
		startWorkers_(corePoolSize_);
	}
	ThreadPoolExecutor::~ThreadPoolExecutor()
	{
		shutdown(); for (auto& w : workers_)if (w.joinable())w.join();
	}
	void ThreadPoolExecutor::startWorkers_(jxx::lang::jint count)
	{
		for (jxx::lang::jint i = 0; i < count; ++i)workers_.emplace_back([this]
	   {
		   workerLoop_();
	   }); largestPoolSize_ = static_cast<jxx::lang::jint>(workers_.size());
	}
	void ThreadPoolExecutor::workerLoop_()
	{
		for (;;) {
			jxx::Ptr<jxx::lang::Runnable> task;
			{
				std::unique_lock<std::mutex> lock(mutex_); workAvailable_.wait(lock, [&]
			   {
				   return stopNow_ || !queue_.empty() || shutdown_;
			   }); if (stopNow_)break; if (queue_.empty()) {
				   if (shutdown_)break; continue;
			   }task = queue_.front(); queue_.pop_front(); ++activeCount_;
			}try {
				task->run();
			}
			catch (...) {
			}
			{
				std::lock_guard<std::mutex> lock(mutex_); --activeCount_; ++completedTaskCount_; if (shutdown_ && queue_.empty() && activeCount_ == 0)terminated_.notify_all();
			}
		}std::lock_guard<std::mutex> lock(mutex_); if (shutdown_ && queue_.empty() && activeCount_ == 0)terminated_.notify_all();
	}
	void ThreadPoolExecutor::reject_(const jxx::Ptr<jxx::lang::Runnable>& command)
	{
		auto h = handler_; if (h != nullptr) {
			auto self = jxx::CAST<ThreadPoolExecutor>(thisPtr); h->rejectedExecution(command, self); return;
		}throw RejectedExecutionException();
	}
	void ThreadPoolExecutor::execute(const jxx::Ptr<jxx::lang::Runnable>& command)
	{
		if (command == nullptr)throw jxx::lang::NullPointerException();
		{
			std::lock_guard<std::mutex> lock(mutex_); if (shutdown_) {
				reject_(command); return;
			}queue_.push_back(command);
		}workAvailable_.notify_one();
	}
	void ThreadPoolExecutor::shutdown()
	{
		{
			std::lock_guard<std::mutex> lock(mutex_); shutdown_ = true;
		}workAvailable_.notify_all();
	}
	jxx::Ptr<jxx::util::List<jxx::lang::Runnable>> ThreadPoolExecutor::shutdownNow()
	{
		auto result = jxx::NEW<jxx::util::ArrayList<jxx::lang::Runnable>>();
		{
			std::lock_guard<std::mutex> lock(mutex_); shutdown_ = true; stopNow_ = true; while (!queue_.empty()) {
				result->add(queue_.front()); queue_.pop_front();
			}
		}
		workAvailable_.notify_all(); 
		return jxx::CAST<jxx::util::List<jxx::lang::Runnable>>(result);
	}
	jxx::lang::jbool ThreadPoolExecutor::isShutdown()
	{
		std::lock_guard<std::mutex> lock(mutex_); return shutdown_;
	}
	jxx::lang::jbool ThreadPoolExecutor::isTerminated()
	{
		std::lock_guard<std::mutex> lock(mutex_); return shutdown_ && queue_.empty() && activeCount_ == 0;
	}
	jxx::lang::jbool ThreadPoolExecutor::awaitTermination(jxx::lang::jlong timeout, const jxx::Ptr<TimeUnit>& unit)
	{
		if (unit == nullptr)throw jxx::lang::NullPointerException(); std::unique_lock<std::mutex> lock(mutex_); return terminated_.wait_for(lock, unit->toChrono(timeout), [&]
	   {
		   return shutdown_ && queue_.empty() && activeCount_ == 0;
	   });
	}
	jxx::lang::jint ThreadPoolExecutor::getPoolSize()
	{
		std::lock_guard<std::mutex> lock(mutex_); return static_cast<jxx::lang::jint>(workers_.size());
	}
	jxx::lang::jint ThreadPoolExecutor::getActiveCount()
	{
		std::lock_guard<std::mutex> lock(mutex_); return activeCount_;
	}
	jxx::lang::jint ThreadPoolExecutor::getLargestPoolSize()
	{
		std::lock_guard<std::mutex> lock(mutex_); return largestPoolSize_;
	}
	jxx::lang::jlong ThreadPoolExecutor::getCompletedTaskCount()
	{
		std::lock_guard<std::mutex> lock(mutex_); return completedTaskCount_;
	}
	jxx::lang::jlong ThreadPoolExecutor::getTaskCount()
	{
		std::lock_guard<std::mutex> lock(mutex_); return completedTaskCount_ + activeCount_ + static_cast<jxx::lang::jlong>(queue_.size());
	}
	void ThreadPoolExecutor::setRejectedExecutionHandler(const jxx::Ptr<RejectedExecutionHandler>& handler)
	{
		if (handler == nullptr)throw jxx::lang::NullPointerException(); std::lock_guard<std::mutex> lock(mutex_); handler_ = handler;
	}
	jxx::Ptr<RejectedExecutionHandler> ThreadPoolExecutor::getRejectedExecutionHandler()
	{
		std::lock_guard<std::mutex> lock(mutex_); return handler_;
	}
} // namespace
