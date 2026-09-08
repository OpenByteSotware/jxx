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

namespace jxx::lang {

struct Thread::NativeState {
    mutable std::mutex mutex;
    std::condition_variable finishedCondition;
    std::condition_variable interruptCondition;
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
    : Super()
    , state_(std::make_unique<NativeState>()) {

    state_->id = nextThreadId.fetch_add(1);
    state_->name = defaultThreadName(state_->id);
}

Thread::Thread(
    const jxx::Ptr<Runnable>& target)
    : Thread(target, nullptr) {
}

Thread::Thread(
    const jxx::Ptr<String>& name)
    : Thread(nullptr, name) {
}

Thread::Thread(
    const jxx::Ptr<Runnable>& target,
    const jxx::Ptr<String>& name)
    : Thread() {

    state_->target = target;

    if (name != nullptr) {
        state_->name = name;
    }
}

Thread::~Thread() {
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

    self->state_->running.store(true);

    try {
        self->run();
    }
    catch (...) {
        self->state_->running.store(false);
        self->state_->finished.store(true);
        self->state_->finishedCondition.notify_all();
        currentThread_.reset();
        return;
    }

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
    state_->interruptCondition.notify_all();
    state_->finishedCondition.notify_all();
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

    state_->priority.store(priority);
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
