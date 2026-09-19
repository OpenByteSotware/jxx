#pragma once

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <limits>
#include <mutex>
#include <unordered_map>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Thread.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace jxx::util::concurrent::locks {

class StampedLock final
    : public ::jxx::lang::ClassBase<
          StampedLock,
          ::jxx::lang::Object,
          ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        StampedLock, JxxSuper, ::jxx::io::SerializableI>;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<
        StampedLock, JxxSuper, ::jxx::io::SerializableI>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {
        return JxxClassInfoMarker::Class();
    }

    StampedLock() : Super() {}

    ::jxx::lang::jlong writeLock() {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [&] { return !writer_ && readers_ == 0; });
        return grantWrite_();
    }

    ::jxx::lang::jlong tryWriteLock() {
        std::lock_guard<std::mutex> lock(mutex_);
        return (!writer_ && readers_ == 0) ? grantWrite_() : 0;
    }

    ::jxx::lang::jlong tryWriteLock(
        ::jxx::lang::jlong time,
        const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) {
        if (!unit) throw ::jxx::lang::NullPointerException();
        if (::jxx::lang::Thread::interrupted()) {
            throw ::jxx::lang::InterruptedException();
        }
        const auto deadline = std::chrono::steady_clock::now() + unit->toChrono(time);
        std::unique_lock<std::mutex> lock(mutex_);
        while (writer_ || readers_ != 0) {
            const auto now = std::chrono::steady_clock::now();
            if (now >= deadline) return 0;
            condition_.wait_for(lock, std::min(
                deadline - now,
                std::chrono::steady_clock::duration(
                    std::chrono::milliseconds(10))));
            if (::jxx::lang::Thread::interrupted()) {
                throw ::jxx::lang::InterruptedException();
            }
        }
        return grantWrite_();
    }

    ::jxx::lang::jlong writeLockInterruptibly() {
        return tryWriteLock(
            std::numeric_limits<::jxx::lang::jlong>::max() / 4,
            ::jxx::util::concurrent::TimeUnit::NANOSECONDS());
    }

    ::jxx::lang::jlong readLock() {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [&] { return !writer_; });
        return grantRead_();
    }

    ::jxx::lang::jlong tryReadLock() {
        std::lock_guard<std::mutex> lock(mutex_);
        return !writer_ ? grantRead_() : 0;
    }

    ::jxx::lang::jlong tryReadLock(
        ::jxx::lang::jlong time,
        const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) {
        if (!unit) throw ::jxx::lang::NullPointerException();
        if (::jxx::lang::Thread::interrupted()) {
            throw ::jxx::lang::InterruptedException();
        }
        const auto deadline = std::chrono::steady_clock::now() + unit->toChrono(time);
        std::unique_lock<std::mutex> lock(mutex_);
        while (writer_) {
            const auto now = std::chrono::steady_clock::now();
            if (now >= deadline) return 0;
            condition_.wait_for(lock, std::min(
                deadline - now,
                std::chrono::steady_clock::duration(
                    std::chrono::milliseconds(10))));
            if (::jxx::lang::Thread::interrupted()) {
                throw ::jxx::lang::InterruptedException();
            }
        }
        return grantRead_();
    }

    ::jxx::lang::jlong readLockInterruptibly() {
        return tryReadLock(
            std::numeric_limits<::jxx::lang::jlong>::max() / 4,
            ::jxx::util::concurrent::TimeUnit::NANOSECONDS());
    }

    ::jxx::lang::jlong tryOptimisticRead() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return writer_ ? 0 : optimisticStamp_();
    }

    ::jxx::lang::jbool validate(::jxx::lang::jlong stamp) const {
        std::lock_guard<std::mutex> lock(mutex_);
        return stamp != 0 && modeOf_(stamp) == OPTIMISTIC &&
            !writer_ && versionOf_(stamp) == version_;
    }

    void unlockWrite(::jxx::lang::jlong stamp) {
        std::lock_guard<std::mutex> lock(mutex_);
        verifyStamp_(stamp, WRITE);
        active_.erase(stamp);
        writer_ = false;
        ++version_;
        condition_.notify_all();
    }

    void unlockRead(::jxx::lang::jlong stamp) {
        std::lock_guard<std::mutex> lock(mutex_);
        verifyStamp_(stamp, READ);
        active_.erase(stamp);
        --readers_;
        if (readers_ == 0) condition_.notify_all();
    }

    void unlock(::jxx::lang::jlong stamp) {
        const auto mode = modeOf_(stamp);
        if (mode == WRITE) unlockWrite(stamp);
        else if (mode == READ) unlockRead(stamp);
        else throw ::jxx::lang::IllegalMonitorStateException();
    }

    ::jxx::lang::jbool tryUnlockWrite() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!writer_) return false;
        for (auto it = active_.begin(); it != active_.end(); ++it) {
            if (it->second == WRITE) {
                active_.erase(it);
                writer_ = false;
                ++version_;
                condition_.notify_all();
                return true;
            }
        }
        return false;
    }

    ::jxx::lang::jbool tryUnlockRead() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (readers_ == 0) return false;
        for (auto it = active_.begin(); it != active_.end(); ++it) {
            if (it->second == READ) {
                active_.erase(it);
                --readers_;
                if (readers_ == 0) condition_.notify_all();
                return true;
            }
        }
        return false;
    }

    ::jxx::lang::jlong tryConvertToWriteLock(::jxx::lang::jlong stamp) {
        std::lock_guard<std::mutex> lock(mutex_);
        const auto mode = modeOf_(stamp);
        if (mode == WRITE && active_.count(stamp)) return stamp;
        if (mode == READ && active_.count(stamp) && readers_ == 1 && !writer_) {
            active_.erase(stamp);
            readers_ = 0;
            return grantWrite_();
        }
        if (mode == OPTIMISTIC && validateUnlocked_(stamp) && readers_ == 0) {
            return grantWrite_();
        }
        return 0;
    }

    ::jxx::lang::jlong tryConvertToReadLock(::jxx::lang::jlong stamp) {
        std::lock_guard<std::mutex> lock(mutex_);
        const auto mode = modeOf_(stamp);
        if (mode == READ && active_.count(stamp)) return stamp;
        if (mode == WRITE && active_.count(stamp)) {
            active_.erase(stamp);
            writer_ = false;
            ++version_;
            return grantRead_();
        }
        if (mode == OPTIMISTIC && validateUnlocked_(stamp)) return grantRead_();
        return 0;
    }

    ::jxx::lang::jlong tryConvertToOptimisticRead(::jxx::lang::jlong stamp) {
        std::lock_guard<std::mutex> lock(mutex_);
        const auto mode = modeOf_(stamp);
        if (mode == OPTIMISTIC && validateUnlocked_(stamp)) return stamp;
        if (mode == WRITE && active_.count(stamp)) {
            active_.erase(stamp);
            writer_ = false;
            ++version_;
            condition_.notify_all();
            return optimisticStamp_();
        }
        if (mode == READ && active_.count(stamp)) {
            active_.erase(stamp);
            --readers_;
            if (readers_ == 0) condition_.notify_all();
            return optimisticStamp_();
        }
        return 0;
    }

    ::jxx::lang::jbool isWriteLocked() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return writer_;
    }

    ::jxx::lang::jbool isReadLocked() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return readers_ != 0;
    }

    ::jxx::lang::jint getReadLockCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return readers_;
    }

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override {(void)out;}
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override {(void)in;}
    void readObjectNoData() override {}

private:
    static constexpr ::jxx::lang::jlong READ = 1;
    static constexpr ::jxx::lang::jlong WRITE = 2;
    static constexpr ::jxx::lang::jlong OPTIMISTIC = 3;

    ::jxx::lang::jlong grantRead_() {
        ++readers_;
        const auto stamp = nextStamp_(READ);
        active_[stamp] = READ;
        return stamp;
    }

    ::jxx::lang::jlong grantWrite_() {
        writer_ = true;
        const auto stamp = nextStamp_(WRITE);
        active_[stamp] = WRITE;
        return stamp;
    }

    ::jxx::lang::jlong nextStamp_(::jxx::lang::jlong mode) {
        ++sequence_;
        return (static_cast<::jxx::lang::jlong>(sequence_) << 34) |
            (static_cast<::jxx::lang::jlong>(version_) << 2) | mode;
    }

    ::jxx::lang::jlong optimisticStamp_() const {
        return (static_cast<::jxx::lang::jlong>(version_) << 2) | OPTIMISTIC;
    }

    static ::jxx::lang::jlong modeOf_(::jxx::lang::jlong stamp) {
        return stamp & 3;
    }

    static std::uint64_t versionOf_(::jxx::lang::jlong stamp) {
        return static_cast<std::uint64_t>(stamp >> 2) & 0xffffffffULL;
    }

    ::jxx::lang::jbool validateUnlocked_(::jxx::lang::jlong stamp) const {
        return stamp != 0 && modeOf_(stamp) == OPTIMISTIC &&
            !writer_ && versionOf_(stamp) == version_;
    }

    void verifyStamp_(::jxx::lang::jlong stamp, ::jxx::lang::jlong mode) const {
        const auto found = active_.find(stamp);
        if (found == active_.end() || found->second != mode) {
            throw ::jxx::lang::IllegalMonitorStateException();
        }
    }

    mutable std::mutex mutex_;
    std::condition_variable condition_;
    ::jxx::lang::jint readers_ = 0;
    ::jxx::lang::jbool writer_ = false;
    std::uint64_t version_ = 1;
    std::uint64_t sequence_ = 0;
    std::unordered_map<::jxx::lang::jlong, ::jxx::lang::jlong> active_;
};

} // namespace jxx::util::concurrent::locks
