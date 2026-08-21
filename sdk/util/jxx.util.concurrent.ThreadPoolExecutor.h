#pragma once
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <mutex>
#include <thread>
#include <vector>
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.concurrent.AbstractExecutorService.h"
#include "util/jxx.util.concurrent.RejectedExecutionException.h"
#include "util/jxx.util.concurrent.RejectedExecutionHandler.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
namespace jxx::util::concurrent {
class ThreadPoolExecutor
    : public AbstractExecutorService {
private:
    mutable std::mutex mutex_;
    std::condition_variable workAvailable_;
    std::condition_variable terminated_;
    std::deque<jxx::Ptr<jxx::lang::Runnable>> queue_;
    std::vector<std::thread> workers_;
    jxx::lang::jbool shutdown_ = false;
    jxx::lang::jbool stopNow_ = false;
    jxx::lang::jint activeCount_ = 0;
    jxx::lang::jlong completedTaskCount_ = 0;
    jxx::lang::jint largestPoolSize_ = 0;
    jxx::lang::jint corePoolSize_;
    jxx::lang::jint maximumPoolSize_;
    jxx::lang::jlong keepAliveTime_;
    jxx::Ptr<TimeUnit> keepAliveUnit_;
    jxx::Ptr<RejectedExecutionHandler> handler_;

    void workerLoop_();
    void startWorkers_(jxx::lang::jint count);
    void reject_(const jxx::Ptr<jxx::lang::Runnable>& command);
public:
    ThreadPoolExecutor(
        jxx::lang::jint corePoolSize,
        jxx::lang::jint maximumPoolSize,
        jxx::lang::jlong keepAliveTime,
        const jxx::Ptr<TimeUnit>& unit);
    ~ThreadPoolExecutor() override;

    void execute(const jxx::Ptr<jxx::lang::Runnable>& command) override;
    void shutdown() override;
    jxx::Ptr<jxx::util::List<jxx::lang::Runnable>> shutdownNow() override;
    jxx::lang::jbool isShutdown() override;
    jxx::lang::jbool isTerminated() override;
    jxx::lang::jbool awaitTermination(jxx::lang::jlong timeout,const jxx::Ptr<TimeUnit>& unit) override;

    jxx::lang::jint getCorePoolSize() const noexcept { return corePoolSize_; }
    jxx::lang::jint getMaximumPoolSize() const noexcept { return maximumPoolSize_; }
    jxx::lang::jint getPoolSize();
    jxx::lang::jint getActiveCount();
    jxx::lang::jint getLargestPoolSize();
    jxx::lang::jlong getCompletedTaskCount();
    jxx::lang::jlong getTaskCount();
    void setRejectedExecutionHandler(const jxx::Ptr<RejectedExecutionHandler>& handler);
    jxx::Ptr<RejectedExecutionHandler> getRejectedExecutionHandler();
};
} // namespace jxx::util::concurrent
