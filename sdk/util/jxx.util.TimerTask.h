#pragma once

#include <mutex>

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx_types.h"

namespace jxx::util {

class Timer;

class TimerTask
    : public ::jxx::lang::ClassBase<
          TimerTask, ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        TimerTask, JxxSuper, ::jxx::lang::Runnable>;

    TimerTask();
    ~TimerTask() override = default;

    virtual void run() override = 0;
    ::jxx::lang::jbool cancel();
    ::jxx::lang::jlong scheduledExecutionTime() const;

private:
    friend class Timer;
    enum class State { VIRGIN, SCHEDULED, EXECUTED, CANCELLED };

    mutable std::mutex mutex_;
    State state_ = State::VIRGIN;
    ::jxx::lang::jlong scheduledExecutionTime_ = 0;
    ::jxx::lang::jlong nextExecutionTime_ = 0;
    ::jxx::lang::jlong period_ = 0;
    ::jxx::lang::jbool fixedRate_ = false;
};

} // namespace jxx::util
