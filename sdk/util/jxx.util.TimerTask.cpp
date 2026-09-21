#include "util/jxx.util.TimerTask.h"

namespace jxx::util {

TimerTask::TimerTask() : Super() {}

::jxx::lang::jbool TimerTask::cancel() {
    std::lock_guard<std::mutex> lock(mutex_);
    const auto preventsExecution =
        state_ == State::SCHEDULED &&
        (period_ != 0 || scheduledExecutionTime_ == 0);
    state_ = State::CANCELLED;
    return preventsExecution;
}

::jxx::lang::jlong TimerTask::scheduledExecutionTime() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return scheduledExecutionTime_;
}

} // namespace jxx::util
