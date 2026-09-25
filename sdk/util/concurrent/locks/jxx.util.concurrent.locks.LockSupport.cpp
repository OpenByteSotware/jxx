#include "util/concurrent/locks/jxx.util.concurrent.locks.LockSupport.h"

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.Exceptions.h"

namespace jxx::util::concurrent::locks {
namespace {

struct Parker final {
    std::mutex mutex;
    std::condition_variable condition;
    ::jxx::lang::jbool permit = false;
    ::jxx::Ptr<::jxx::lang::Object> blocker;
};

std::mutex registryMutex;
std::unordered_map<::jxx::lang::jlong, std::shared_ptr<Parker>> registry;

std::shared_ptr<Parker> parkerFor(::jxx::lang::jlong id) {
    std::lock_guard<std::mutex> lock(registryMutex);
    auto& parker = registry[id];
    if (!parker) {
        parker = std::make_shared<Parker>();
    }
    return parker;
}

::jxx::Ptr<::jxx::lang::Thread> currentThread() {
    auto thread = ::jxx::lang::Thread::currentThread();
    if (!thread) {
        throw ::jxx::lang::IllegalStateException();
    }
    return thread;
}

::jxx::lang::jbool shouldReturn(const ::jxx::Ptr<::jxx::lang::Thread>& thread,
                   const std::shared_ptr<Parker>& parker) {
    return parker->permit || thread->isInterrupted();
}

template<typename DeadlinePredicate>
void parkImpl(const ::jxx::Ptr<::jxx::lang::Object>& blocker,
              DeadlinePredicate waitStep) {
    auto thread = currentThread();
    auto parker = parkerFor(thread->getId());
    std::unique_lock<std::mutex> lock(parker->mutex);

    if (parker->permit) {
        parker->permit = false;
        return;
    }
    if (thread->isInterrupted()) {
        return;
    }

    parker->blocker = blocker;
    std::weak_ptr<Parker> weakParker = parker;
    thread->setParkWakeup_([weakParker] {
        if (auto currentParker = weakParker.lock()) {
            currentParker->condition.notify_one();
        }
    });

    while (!shouldReturn(thread, parker) && waitStep(*parker, lock)) {
    }

    thread->clearParkWakeup_();
    parker->blocker.reset();
    if (parker->permit) {
        parker->permit = false;
    }
}

} // namespace

::jxx::Ptr<::jxx::lang::ClassAny> LockSupport::Class() {
    return JxxClassInfoMarker::Class();
}

void LockSupport::unpark(const ::jxx::Ptr<::jxx::lang::Thread>& thread) {
    if (!thread) {
        return;
    }
    auto parker = parkerFor(thread->getId());
    {
        std::lock_guard<std::mutex> lock(parker->mutex);
        parker->permit = true;
    }
    parker->condition.notify_one();
}

void LockSupport::park() {
    park(nullptr);
}

void LockSupport::park(const ::jxx::Ptr<::jxx::lang::Object>& blocker) {
    parkImpl(blocker, [](Parker& parker, std::unique_lock<std::mutex>& lock) {
        parker.condition.wait(lock);
        return true;
    });
}

void LockSupport::parkNanos(::jxx::lang::jlong nanos) {
    parkNanos(nullptr, nanos);
}

void LockSupport::parkNanos(const ::jxx::Ptr<::jxx::lang::Object>& blocker,
                            ::jxx::lang::jlong nanos) {
    if (nanos <= 0) {
        return;
    }
    const auto deadline = std::chrono::steady_clock::now() +
        std::chrono::nanoseconds(nanos);
    parkImpl(blocker, [deadline](Parker& parker,
                                std::unique_lock<std::mutex>& lock) {
        const auto now = std::chrono::steady_clock::now();
        if (now >= deadline) {
            return false;
        }
        const auto remaining = deadline - now;
        parker.condition.wait_for(
            lock,
            std::min(remaining,
                     std::chrono::steady_clock::duration(
                         std::chrono::milliseconds(10))));
        return std::chrono::steady_clock::now() < deadline;
    });
}

void LockSupport::parkUntil(::jxx::lang::jlong deadline) {
    parkUntil(nullptr, deadline);
}

void LockSupport::parkUntil(const ::jxx::Ptr<::jxx::lang::Object>& blocker,
                            ::jxx::lang::jlong deadline) {
    const auto target = std::chrono::system_clock::time_point(
        std::chrono::milliseconds(deadline));
    parkImpl(blocker, [target](Parker& parker,
                              std::unique_lock<std::mutex>& lock) {
        const auto now = std::chrono::system_clock::now();
        if (now >= target) {
            return false;
        }
        const auto remaining = target - now;
        parker.condition.wait_for(
            lock,
            std::min(remaining,
                     std::chrono::system_clock::duration(
                         std::chrono::milliseconds(10))));
        return std::chrono::system_clock::now() < target;
    });
}

::jxx::Ptr<::jxx::lang::Object> LockSupport::getBlocker(
    const ::jxx::Ptr<::jxx::lang::Thread>& thread) {
    if (!thread) {
        throw ::jxx::lang::NullPointerException();
    }
    auto parker = parkerFor(thread->getId());
    std::lock_guard<std::mutex> lock(parker->mutex);
    return parker->blocker;
}

} // namespace jxx::util::concurrent::locks
