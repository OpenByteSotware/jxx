#pragma once

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <mutex>
#include <thread>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Thread.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.Lock.h"
#include "util/jxx.util.Date.h"

namespace jxx::util::concurrent::locks {

class ReentrantLock final
    : public ::jxx::lang::ClassBase<
          ReentrantLock,
          ::jxx::lang::Object,
          Lock,
          ::jxx::io::SerializableI> {
private:
    class ConditionImpl final
        : public ::jxx::lang::ClassBase<
              ConditionImpl,
              ::jxx::lang::Object,
              Condition> {
    public:
        using Super = ::jxx::lang::ClassBase<
            ConditionImpl,
            ::jxx::lang::Object,
            Condition>;

        explicit ConditionImpl(ReentrantLock* owner)
            : Super(), owner_(owner) {}

        void await() override {
            if (::jxx::lang::Thread::interrupted()) {
                throw ::jxx::lang::InterruptedException();
            }
            waitInterruptibly_(nullptr);
        }

        void awaitUninterruptibly() override {
            auto current = ::jxx::lang::Thread::currentThread();
            ::jxx::lang::jbool interrupted = false;
            owner_->checkOwner_();

            std::unique_lock<std::mutex> conditionLock(mutex_);
            const auto observed = generation_;
            ++waiters_;
            const auto holds = owner_->releaseFully_();
            while (generation_ == observed) {
                condition_.wait_for(conditionLock, std::chrono::milliseconds(10));
                if (current && current->isInterrupted()) interrupted = true;
            }
            if (waiters_ > 0) --waiters_;
            conditionLock.unlock();
            owner_->reacquire_(holds);
            if (interrupted && current) current->interrupt();
        }

        ::jxx::lang::jlong awaitNanos(
            ::jxx::lang::jlong nanosTimeout) override {
            const auto deadline = std::chrono::steady_clock::now() +
                std::chrono::nanoseconds(nanosTimeout);
            const auto signalled = waitInterruptibly_(&deadline);
            const auto remaining = std::chrono::duration_cast<std::chrono::nanoseconds>(
                deadline - std::chrono::steady_clock::now()).count();
            return signalled ? remaining : std::min<::jxx::lang::jlong>(remaining, 0);
        }

        ::jxx::lang::jbool await(
            ::jxx::lang::jlong time,
            const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) override {
            if (!unit) throw ::jxx::lang::NullPointerException();
            const auto deadline = std::chrono::steady_clock::now() +
                unit->toChrono(time);
            return waitInterruptibly_(&deadline);
        }

        ::jxx::lang::jbool awaitUntil(
            const ::jxx::Ptr<::jxx::util::Date>& deadline) override {
            if (!deadline) throw ::jxx::lang::NullPointerException();
            const auto systemDeadline = std::chrono::system_clock::time_point(
                std::chrono::milliseconds(deadline->getTime()));
            const auto remaining = systemDeadline - std::chrono::system_clock::now();
            const auto steadyDeadline = std::chrono::steady_clock::now() +
                std::chrono::duration_cast<std::chrono::steady_clock::duration>(remaining);
            return waitInterruptibly_(&steadyDeadline);
        }

        void signal() override {
            owner_->checkOwner_();
            {
                std::lock_guard<std::mutex> lock(mutex_);
                ++generation_;
            }
            condition_.notify_one();
        }

        void signalAll() override {
            owner_->checkOwner_();
            {
                std::lock_guard<std::mutex> lock(mutex_);
                ++generation_;
            }
            condition_.notify_all();
        }

    private:
        ::jxx::lang::jbool waitInterruptibly_(
            const std::chrono::steady_clock::time_point* deadline) {
            owner_->checkOwner_();
            if (::jxx::lang::Thread::interrupted()) {
                throw ::jxx::lang::InterruptedException();
            }

            std::unique_lock<std::mutex> conditionLock(mutex_);
            const auto observed = generation_;
            const auto holds = owner_->releaseFully_();
            ::jxx::lang::jbool signalled = false;
            ::jxx::lang::jbool interrupted = false;

            while (generation_ == observed) {
                if (deadline) {
                    const auto now = std::chrono::steady_clock::now();
                    if (now >= *deadline) break;
                    condition_.wait_for(
                        conditionLock,
                        std::min(*deadline - now,
                            std::chrono::steady_clock::duration(
                                std::chrono::milliseconds(10))));
                } else {
                    condition_.wait_for(conditionLock, std::chrono::milliseconds(10));
                }
                if (auto current = ::jxx::lang::Thread::currentThread();
                    current && current->isInterrupted()) {
                    interrupted = true;
                    break;
                }
            }
            signalled = generation_ != observed;
            if (waiters_ > 0) --waiters_;
            conditionLock.unlock();
            owner_->reacquire_(holds);

            if (interrupted) {
                (void)::jxx::lang::Thread::interrupted();
                throw ::jxx::lang::InterruptedException();
            }
            return signalled;
        }

        ReentrantLock* owner_;
        mutable std::mutex mutex_;
        std::condition_variable condition_;
        std::uint64_t generation_ = 0;
        ::jxx::lang::jint waiters_ = 0;

    public:
        ::jxx::lang::jint waiterCount() const {
            std::lock_guard<std::mutex> lock(mutex_);
            return waiters_;
        }
        ReentrantLock* owner() const noexcept { return owner_; }
    };

public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        ReentrantLock, JxxSuper, Lock, ::jxx::io::SerializableI>;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<
        ReentrantLock, JxxSuper, Lock, ::jxx::io::SerializableI>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {
        return JxxClassInfoMarker::Class();
    }

    ReentrantLock() : ReentrantLock(false) {}
    explicit ReentrantLock(::jxx::lang::jbool fair)
        : Super(), fair_(fair) {}

    void lock() override {
        if (isHeldByCurrentThread()) {
            mutex_.lock();
            acquired_();
            return;
        }
        if (fair_) {
            (void)acquireFair_(false, nullptr);
        } else {
            mutex_.lock();
            acquired_();
        }
    }

    void lockInterruptibly() override {
        if (::jxx::lang::Thread::interrupted()) {
            throw ::jxx::lang::InterruptedException();
        }
        if (isHeldByCurrentThread()) {
            mutex_.lock();
            acquired_();
            return;
        }
        if (fair_) {
            (void)acquireFair_(true, nullptr);
            return;
        }
        while (!mutex_.try_lock_for(std::chrono::milliseconds(10))) {
            if (::jxx::lang::Thread::interrupted()) {
                throw ::jxx::lang::InterruptedException();
            }
        }
        acquired_();
    }

    ::jxx::lang::jbool tryLock() override {
        if (!mutex_.try_lock()) return false;
        acquired_();
        return true;
    }

    ::jxx::lang::jbool tryLock(
        ::jxx::lang::jlong time,
        const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) override {
        if (!unit) throw ::jxx::lang::NullPointerException();
        if (::jxx::lang::Thread::interrupted()) {
            throw ::jxx::lang::InterruptedException();
        }
        const auto deadline = std::chrono::steady_clock::now() + unit->toChrono(time);
        if (fair_ && !isHeldByCurrentThread()) {
            return acquireFair_(true, &deadline);
        }
        for (;;) {
            const auto now = std::chrono::steady_clock::now();
            if (now >= deadline) return false;
            if (mutex_.try_lock_for(std::min(
                    deadline - now,
                    std::chrono::steady_clock::duration(
                        std::chrono::milliseconds(10))))) {
                acquired_();
                return true;
            }
            if (::jxx::lang::Thread::interrupted()) {
                throw ::jxx::lang::InterruptedException();
            }
        }
    }

    void unlock() override {
        checkOwner_();
        if (--holdCount_ == 0) {
            owner_ = std::thread::id{};
            ownerThread_.reset();
        }
        mutex_.unlock();
        admissionChanged_.notify_all();
    }

    ::jxx::Ptr<Condition> newCondition() override {
        return ::jxx::CAST<Condition>(::jxx::NEW<ConditionImpl>(this));
    }

    ::jxx::lang::jint getHoldCount() const noexcept {
        return owner_ == std::this_thread::get_id() ? holdCount_ : 0;
    }
    ::jxx::lang::jbool isHeldByCurrentThread() const noexcept {
        return owner_ == std::this_thread::get_id();
    }
    ::jxx::lang::jbool isLocked() const noexcept { return holdCount_ > 0; }
    ::jxx::lang::jbool isFair() const noexcept { return fair_; }

    ::jxx::Ptr<::jxx::lang::Thread> getOwner() const {
        return ownerThread_;
    }

    ::jxx::lang::jbool hasQueuedThreads() const {
        std::lock_guard<std::mutex> lock(admissionMutex_);
        return !admissionQueue_.empty();
    }

    ::jxx::lang::jbool hasQueuedThread(
        const ::jxx::Ptr<::jxx::lang::Thread>& thread) const {
        if (!thread) throw ::jxx::lang::NullPointerException();
        std::lock_guard<std::mutex> lock(admissionMutex_);
        return std::find(admissionQueue_.begin(), admissionQueue_.end(),
            thread->getId()) != admissionQueue_.end();
    }

    ::jxx::lang::jint getQueueLength() const {
        std::lock_guard<std::mutex> lock(admissionMutex_);
        return static_cast<::jxx::lang::jint>(admissionQueue_.size());
    }

    ::jxx::lang::jbool hasWaiters(const ::jxx::Ptr<Condition>& condition) const {
        return getWaitQueueLength(condition) > 0;
    }

    ::jxx::lang::jint getWaitQueueLength(
        const ::jxx::Ptr<Condition>& condition) const {
        if (!condition) throw ::jxx::lang::NullPointerException();
        auto concrete = std::dynamic_pointer_cast<ConditionImpl>(condition);
        if (!concrete || concrete->owner() != this) {
            throw ::jxx::lang::IllegalArgumentException();
        }
        return concrete->waiterCount();
    }

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override {(void)out;}
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override {(void)in;}
    void readObjectNoData() override {}

private:
    void acquired_() {
        if (owner_ == std::this_thread::get_id()) ++holdCount_;
        else {
            owner_ = std::this_thread::get_id();
            ownerThread_ = ::jxx::lang::Thread::currentThread();
            holdCount_ = 1;
        }
    }

    void checkOwner_() const {
        if (owner_ != std::this_thread::get_id()) {
            throw ::jxx::lang::IllegalMonitorStateException();
        }
    }

    ::jxx::lang::jint releaseFully_() {
        checkOwner_();
        const auto holds = holdCount_;
        owner_ = std::thread::id{};
        holdCount_ = 0;
        for (::jxx::lang::jint i = 0; i < holds; ++i) mutex_.unlock();
        return holds;
    }

    void reacquire_(::jxx::lang::jint holds) {
        for (::jxx::lang::jint i = 0; i < holds; ++i) mutex_.lock();
        owner_ = std::this_thread::get_id();
        ownerThread_ = ::jxx::lang::Thread::currentThread();
        holdCount_ = holds;
    }

    ::jxx::lang::jbool acquireFair_(
        ::jxx::lang::jbool interruptible,
        const std::chrono::steady_clock::time_point* deadline) {
        auto current = ::jxx::lang::Thread::currentThread();
        const auto id = current ? current->getId() : 0;
        {
            std::lock_guard<std::mutex> lock(admissionMutex_);
            admissionQueue_.push_back(id);
        }
        for (;;) {
            if (interruptible && ::jxx::lang::Thread::interrupted()) {
                std::lock_guard<std::mutex> lock(admissionMutex_);
                auto it=std::find(admissionQueue_.begin(),admissionQueue_.end(),id);
                if(it!=admissionQueue_.end()) admissionQueue_.erase(it);
                admissionChanged_.notify_all();
                throw ::jxx::lang::InterruptedException();
            }
            if (deadline && std::chrono::steady_clock::now() >= *deadline) {
                std::lock_guard<std::mutex> lock(admissionMutex_);
                auto it=std::find(admissionQueue_.begin(),admissionQueue_.end(),id);
                if(it!=admissionQueue_.end()) admissionQueue_.erase(it);
                admissionChanged_.notify_all();
                return false;
            }
            bool first=false;
            {
                std::lock_guard<std::mutex> lock(admissionMutex_);
                first=!admissionQueue_.empty() && admissionQueue_.front()==id;
            }
            if (first && mutex_.try_lock_for(std::chrono::milliseconds(10))) {
                std::lock_guard<std::mutex> lock(admissionMutex_);
                admissionQueue_.pop_front();
                acquired_();
                admissionChanged_.notify_all();
                return true;
            }
            std::unique_lock<std::mutex> lock(admissionMutex_);
            admissionChanged_.wait_for(lock,std::chrono::milliseconds(10));
        }
    }

    mutable std::recursive_timed_mutex mutex_;
    std::thread::id owner_;
    ::jxx::Ptr<::jxx::lang::Thread> ownerThread_;
    ::jxx::lang::jint holdCount_ = 0;
    mutable std::mutex admissionMutex_;
    std::condition_variable admissionChanged_;
    std::deque<::jxx::lang::jlong> admissionQueue_;
    ::jxx::lang::jbool fair_;
};

} // namespace jxx::util::concurrent::locks
