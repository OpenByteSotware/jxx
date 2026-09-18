#include "lang/jxx.lang.Thread.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <utility>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.ThreadLocalSupport.h"
#include "lang/jxx.lang.ThreadGroup.h"

namespace jxx::lang {

struct Thread::NativeState {
    mutable std::mutex mutex;
    std::condition_variable finishedCondition;
    std::condition_variable interruptCondition;
    std::function<void()> parkWakeup;
    std::thread nativeThread;

    jxx::Ptr<Runnable> target;
    jxx::Ptr<String> name;

    std::atomic<jbool> started{false};
    std::atomic<jbool> running{false};
    std::atomic<jbool> finished{false};
    std::atomic<jbool> interrupted{false};

    std::atomic<jint> priority{Thread::NORM_PRIORITY};
    std::atomic<jbool> daemon{false};

    jlong id = 0;
    jxx::Ptr<ThreadGroup> group;
    std::vector<std::function<void()>> inheritedValues;
};

thread_local std::weak_ptr<Thread>
Thread::currentThread_;

namespace {

std::atomic<jlong> nextThreadId{1};

jxx::Ptr<String> defaultThreadName(jlong id) {
    return jxx::NEW<String>(
        std::string("Thread-") +
        std::to_string(id));
}

void validateDelay(jlong millis, jint nanos) {
    if (millis < 0 || nanos < 0 || nanos > 999999) {
        throw IllegalArgumentException();
    }
}

std::chrono::nanoseconds toDuration(
    jlong millis,
    jint nanos) {

    validateDelay(millis, nanos);

    return std::chrono::milliseconds(millis) +
        std::chrono::nanoseconds(nanos);
}

} // namespace

Thread::Thread()
    : Thread(jxx::Ptr<ThreadGroup>{}, jxx::Ptr<Runnable>{}, jxx::Ptr<String>{}, 0) {}
Thread::Thread(const jxx::Ptr<Runnable>& target)
    : Thread(jxx::Ptr<ThreadGroup>{}, target, jxx::Ptr<String>{}, 0) {}
Thread::Thread(const jxx::Ptr<String>& name)
    : Thread(jxx::Ptr<ThreadGroup>{}, jxx::Ptr<Runnable>{}, name, 0) {}
Thread::Thread(const jxx::Ptr<Runnable>& target, const jxx::Ptr<String>& name)
    : Thread(jxx::Ptr<ThreadGroup>{}, target, name, 0) {}
Thread::Thread(const jxx::Ptr<ThreadGroup>& group, const jxx::Ptr<Runnable>& target)
    : Thread(group, target, jxx::Ptr<String>{}, 0) {}
Thread::Thread(const jxx::Ptr<ThreadGroup>& group, const jxx::Ptr<String>& name)
    : Thread(group, jxx::Ptr<Runnable>{}, name, 0) {}
Thread::Thread(const jxx::Ptr<ThreadGroup>& group, const jxx::Ptr<Runnable>& target,
               const jxx::Ptr<String>& name)
    : Thread(group, target, name, 0) {}
Thread::Thread(const jxx::Ptr<ThreadGroup>& group, const jxx::Ptr<Runnable>& target,
               const jxx::Ptr<String>& name, jlong stackSize)
    : Super(), state_(std::make_unique<NativeState>()) {
    (void)stackSize;
    state_->target = target;
    state_->id = nextThreadId.fetch_add(1);
    state_->name = name == nullptr ? defaultThreadName(state_->id) : name;
    auto current = currentThread();
    state_->group = group != nullptr ? group
        : (current != nullptr && current->state_->group != nullptr
            ? current->state_->group : ThreadGroup::systemThreadGroup());
    state_->priority.store(std::min(state_->priority.load(), state_->group->getMaxPriority()));
    state_->group->addThread_(this);
    state_->inheritedValues = thread_local_detail::captureInheritedValues();
}

Thread::~Thread() {
    if (state_ != nullptr && state_->group != nullptr) state_->group->removeThread_(this);
    if (state_ != nullptr &&
        state_->nativeThread.joinable()) {

        state_->nativeThread.detach();
    }
}

void Thread::start() {
    jbool expected = false;

    if (!state_->started.compare_exchange_strong(
            expected,
            true)) {

        throw IllegalStateException();
    }

    auto self =
        jxx::CAST<Thread>(thisPtr());

    state_->nativeThread =
        std::thread(&Thread::entry_, self);
}

void Thread::entry_(
    const jxx::Ptr<Thread>& self) {

    currentThread_ = self;

    for (auto& install : self->state_->inheritedValues) {
        install();
    }
    self->state_->inheritedValues.clear();

    self->state_->running.store(true);

    try {
        self->run();
    }
    catch (const Throwable& throwable) {
        try {
            self->state_->group->uncaughtException(self, throwable.cloneThrowable());
        }
        catch (...) {
        }
        thread_local_detail::clearCurrentThreadValues();
        self->state_->group->removeThread_(self.get());
        self->state_->running.store(false);
        self->state_->finished.store(true);
        self->state_->finishedCondition.notify_all();
        currentThread_.reset();
        return;
    }
    catch (...) {
        thread_local_detail::clearCurrentThreadValues();
        self->state_->group->removeThread_(self.get());
        self->state_->running.store(false);
        self->state_->finished.store(true);
        self->state_->finishedCondition.notify_all();
        currentThread_.reset();
        return;
    }

    thread_local_detail::clearCurrentThreadValues();
    self->state_->group->removeThread_(self.get());
    self->state_->running.store(false);
    self->state_->finished.store(true);
    self->state_->finishedCondition.notify_all();
    currentThread_.reset();
}

void Thread::run() {
    if (state_->target != nullptr) {
        state_->target->run();
    }
}

void Thread::interrupt() {
    state_->interrupted.store(true);

    std::function<void()> parkWakeup;
    {
        std::lock_guard<std::mutex> lock(state_->mutex);
        parkWakeup = state_->parkWakeup;
    }

    state_->interruptCondition.notify_all();
    state_->finishedCondition.notify_all();

    if (parkWakeup) {
        parkWakeup();
    }
}

void Thread::setParkWakeup_(const std::function<void()>& wakeup) {
    std::lock_guard<std::mutex> lock(state_->mutex);
    state_->parkWakeup = wakeup;
}

void Thread::clearParkWakeup_() {
    std::lock_guard<std::mutex> lock(state_->mutex);
    state_->parkWakeup = nullptr;
}

jbool Thread::isInterrupted() const {
    return state_->interrupted.load();
}

jbool Thread::interrupted() {
    auto current = currentThread();

    if (current == nullptr) {
        return false;
    }

    return current->state_->interrupted.exchange(false);
}

jbool Thread::isAlive() const {
    return state_->running.load();
}

void Thread::join() {
    if (state_->nativeThread.joinable()) {
        state_->nativeThread.join();
    }
}

void Thread::join(jlong millis) {
    join(millis, 0);
}

void Thread::join(jlong millis, jint nanos) {
    const auto duration =
        toDuration(millis, nanos);

    if (duration.count() == 0) {
        join();
        return;
    }

    std::unique_lock<std::mutex> lock(
        state_->mutex);

    const bool finished =
        state_->finishedCondition.wait_for(
            lock,
            duration,
            [this] {
                return state_->finished.load();
            });

    lock.unlock();

    if (finished && state_->nativeThread.joinable()) {
        state_->nativeThread.join();
    }
}

void Thread::sleep(jlong millis) {
    sleep(millis, 0);
}

void Thread::sleep(jlong millis, jint nanos) {
    const auto duration =
        toDuration(millis, nanos);

    auto current = currentThread();

    if (current == nullptr) {
        std::this_thread::sleep_for(duration);
        return;
    }

    if (current->state_->interrupted.exchange(false)) {
        throw InterruptedException();
    }

    std::unique_lock<std::mutex> lock(
        current->state_->mutex);

    const bool interruptedDuringWait =
        current->state_->interruptCondition.wait_for(
            lock,
            duration,
            [&current] {
                return current->state_->interrupted.load();
            });

    if (interruptedDuringWait) {
        current->state_->interrupted.store(false);
        throw InterruptedException();
    }
}

void Thread::yield() {
    std::this_thread::yield();
}

jxx::Ptr<Thread> Thread::currentThread() {
    return currentThread_.lock();
}

jlong Thread::getId() const {
    return state_->id;
}

jxx::Ptr<ThreadGroup> Thread::getThreadGroup() const {
    return state_->finished.load() ? nullptr : state_->group;
}

jxx::Ptr<String> Thread::getName() const {
    std::lock_guard<std::mutex> lock(
        state_->mutex);

    return state_->name;
}

void Thread::setName(
    const jxx::Ptr<String>& name) {

    if (name == nullptr) {
        throw NullPointerException();
    }

    std::lock_guard<std::mutex> lock(
        state_->mutex);

    state_->name = name;
}

jint Thread::getPriority() const {
    return state_->priority.load();
}

void Thread::setPriority(jint priority) {
    if (priority < MIN_PRIORITY ||
        priority > MAX_PRIORITY) {

        throw IllegalArgumentException();
    }

    state_->priority.store(std::min(priority, state_->group->getMaxPriority()));
}

jbool Thread::isDaemon() const {
    return state_->daemon.load();
}

void Thread::setDaemon(jbool daemon) {
    if (state_->started.load()) {
        throw IllegalStateException();
    }

    state_->daemon.store(daemon);
}

Thread::State Thread::getState() const {
    if (!state_->started.load()) {
        return State::NEW;
    }

    if (state_->finished.load()) {
        return State::TERMINATED;
    }

    return State::RUNNABLE;
}

jxx::Ptr<String> Thread::toString() const {
    std::ostringstream output;

    output << "Thread["
           << getName()->utf8()
           << ','
           << getPriority()
           << ']';

    return jxx::NEW<String>(
        output.str());
}

} // namespace jxx::lang
