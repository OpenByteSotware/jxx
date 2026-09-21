#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimerTask.h"

namespace jxx::util {

class Timer final : public ::jxx::lang::ClassBase<Timer, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<Timer, JxxSuper>;

    Timer();
    explicit Timer(::jxx::lang::jbool isDaemon);
    explicit Timer(const ::jxx::Ptr<::jxx::lang::String>& name);
    Timer(const ::jxx::Ptr<::jxx::lang::String>& name,
          ::jxx::lang::jbool isDaemon);
    ~Timer() override;

    void schedule(const ::jxx::Ptr<TimerTask>& task,
                  ::jxx::lang::jlong delay);
    void schedule(const ::jxx::Ptr<TimerTask>& task,
                  const ::jxx::Ptr<Date>& time);
    void schedule(const ::jxx::Ptr<TimerTask>& task,
                  ::jxx::lang::jlong delay,
                  ::jxx::lang::jlong period);
    void schedule(const ::jxx::Ptr<TimerTask>& task,
                  const ::jxx::Ptr<Date>& firstTime,
                  ::jxx::lang::jlong period);
    void scheduleAtFixedRate(const ::jxx::Ptr<TimerTask>& task,
                             ::jxx::lang::jlong delay,
                             ::jxx::lang::jlong period);
    void scheduleAtFixedRate(const ::jxx::Ptr<TimerTask>& task,
                             const ::jxx::Ptr<Date>& firstTime,
                             ::jxx::lang::jlong period);
    void cancel();
    ::jxx::lang::jint purge();

private:
    struct Later {
        bool operator()(const ::jxx::Ptr<TimerTask>& left,
                        const ::jxx::Ptr<TimerTask>& right) const;
    };
    void schedule_(const ::jxx::Ptr<TimerTask>& task,
                   ::jxx::lang::jlong firstTime,
                   ::jxx::lang::jlong period,
                   ::jxx::lang::jbool fixedRate);
    void worker_();
    static ::jxx::lang::jlong now_();

    std::mutex mutex_;
    std::condition_variable changed_;
    std::priority_queue<::jxx::Ptr<TimerTask>,
        std::vector<::jxx::Ptr<TimerTask>>, Later> queue_;
    std::thread workerThread_;
    ::jxx::lang::jbool cancelled_ = false;
    ::jxx::lang::jbool daemon_ = false;
    std::string name_;
};

} // namespace jxx::util
