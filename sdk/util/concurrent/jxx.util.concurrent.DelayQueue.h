#pragma once

#include <condition_variable>
#include <cstdint>
#include <deque>
#include <mutex>
#include <limits>
#include <algorithm>
#include "lang/jxx.lang.Comparable.h"
#include "util/jxx.util.concurrent.Delayed.h"
#include "util/jxx.util.Comparator.h"
#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Thread.h"
#include "util/jxx.util.AbstractQueue.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.concurrent.BlockingQueue.h"

namespace jxx::util::concurrent {

template<typename E>
class DelayQueue final
    : public ::jxx::lang::ClassBase<
          DelayQueue<E>,
          ::jxx::util::AbstractQueue<E>,
          BlockingQueue<E>,
          ::jxx::io::SerializableI> {
private:
    class SnapshotIterator final
        : public ::jxx::lang::ClassBase<
              SnapshotIterator,
              ::jxx::lang::Object,
              ::jxx::util::Iterator<E>> {
    public:
        explicit SnapshotIterator(std::vector<::jxx::Ptr<E>> values)
            : values_(std::move(values)) {}
        ::jxx::lang::jbool hasNext() override { return index_ < values_.size(); }
        ::jxx::Ptr<E> next() override {
            if (!hasNext()) throw ::jxx::util::NoSuchElementException();
            return values_[index_++];
        }
        void remove() override { throw ::jxx::lang::UnsupportedOperationException(); }
    private:
        std::vector<::jxx::Ptr<E>> values_;
        std::size_t index_ = 0;
    };

public:
    using JxxSuper = ::jxx::util::AbstractQueue<E>;
    using Super = ::jxx::lang::ClassBase<
        DelayQueue<E>, JxxSuper, BlockingQueue<E>,
        ::jxx::io::SerializableI>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {
        return JxxClassInfoMarker::Class();
    }

    DelayQueue() : Super() {}

    ::jxx::lang::jbool offer(const ::jxx::Ptr<E>& element) override {
        requireElement_(element);
        std::lock_guard<std::mutex> lock(mutex_);
        auto position = std::lower_bound(queue_.begin(), queue_.end(), element,
            [this](const ::jxx::Ptr<E>& left, const ::jxx::Ptr<E>& right) {
                return compare_(left, right) < 0;
            });
        queue_.insert(position, element);
        notEmpty_.notify_one();
        return true;
    }

    void put(const ::jxx::Ptr<E>& element) override {
        if (::jxx::lang::Thread::interrupted()) throw ::jxx::lang::InterruptedException();
        (void)offer(element);
    }

    ::jxx::lang::jbool offer(
        const ::jxx::Ptr<E>& element,
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) override {
        (void)timeout;
        if (unit == nullptr) throw ::jxx::lang::NullPointerException();
        return offer(element);
    }

    ::jxx::Ptr<E> poll() override {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty() || delay_(queue_.front()) > 0) return nullptr;
        auto result = queue_.front();
        queue_.pop_front();
        notFull_.notify_one();
        return result;
    }

    ::jxx::Ptr<E> take() override {
        if (::jxx::lang::Thread::interrupted()) throw ::jxx::lang::InterruptedException();
        std::unique_lock<std::mutex> lock(mutex_);
        for (;;) {
            if (queue_.empty()) { notEmpty_.wait(lock); continue; }
            const auto delay = delay_(queue_.front());
            if (delay <= 0) { auto result=queue_.front(); queue_.pop_front(); return result; }
            notEmpty_.wait_for(lock, std::chrono::nanoseconds(delay));
        }
    }

    ::jxx::Ptr<E> poll(::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) override {
        if (unit == nullptr) throw ::jxx::lang::NullPointerException();
        auto remaining=unit->toChrono(timeout);
        std::unique_lock<std::mutex> lock(mutex_);
        const auto deadline=std::chrono::steady_clock::now()+remaining;
        for (;;) {
            if (!queue_.empty()) {
                const auto delay=delay_(queue_.front());
                if (delay<=0) { auto result=queue_.front(); queue_.pop_front(); return result; }
                remaining=std::chrono::duration_cast<std::chrono::nanoseconds>(deadline-std::chrono::steady_clock::now());
                if (remaining.count()<=0) return nullptr;
                notEmpty_.wait_for(lock, std::min(remaining,std::chrono::nanoseconds(delay)));
            } else if (notEmpty_.wait_until(lock,deadline)==std::cv_status::timeout) return nullptr;
        }
    }

    ::jxx::Ptr<E> peek() override {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty() ? nullptr : queue_.front();
    }

    ::jxx::lang::jint size() override {
        std::lock_guard<std::mutex> lock(mutex_);
        return static_cast<::jxx::lang::jint>(queue_.size());
    }

    ::jxx::lang::jint remainingCapacity() override {
        std::lock_guard<std::mutex> lock(mutex_);
        return std::numeric_limits<::jxx::lang::jint>::max();
    }

    ::jxx::Ptr<::jxx::util::Iterator<E>> iterator() override {
        std::vector<::jxx::Ptr<E>> values;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            values.assign(queue_.begin(), queue_.end());
        }
        return ::jxx::CAST<::jxx::util::Iterator<E>>(
            ::jxx::NEW<SnapshotIterator>(std::move(values)));
    }

    void clear() override {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.clear();
        notFull_.notify_all();
    }

    ::jxx::lang::jbool remove(
        const ::jxx::Ptr<::jxx::lang::Object>& object) override {
        if (object == nullptr) return false;
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto iterator = queue_.begin(); iterator != queue_.end(); ++iterator) {
            if ((*iterator)->equals(object)) {
                queue_.erase(iterator);
                notFull_.notify_one();
                return true;
            }
        }
        return false;
    }

    ::jxx::lang::jint drainTo(
        const ::jxx::Ptr<::jxx::util::Collection<E>>& target) override {
        return drainTo(target, std::numeric_limits<::jxx::lang::jint>::max());
    }

    ::jxx::lang::jint drainTo(
        const ::jxx::Ptr<::jxx::util::Collection<E>>& target,
        ::jxx::lang::jint maximumElements) override {
        if (target == nullptr) throw ::jxx::lang::NullPointerException();
        if (target.get() == static_cast<::jxx::util::Collection<E>*>(this)) {
            throw ::jxx::lang::IllegalArgumentException();
        }
        if (maximumElements <= 0) return 0;
        std::lock_guard<std::mutex> lock(mutex_);
        ::jxx::lang::jint count = 0;
        while (!queue_.empty() && delay_(queue_.front()) <= 0 && count < maximumElements) {
            target->add(queue_.front());
            queue_.pop_front();
            ++count;
        }
        if (count > 0) notFull_.notify_all();
        return count;
    }

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override { (void)out; }
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override { (void)in; }
    void readObjectNoData() override {}

private:
    static void requireElement_(const ::jxx::Ptr<E>& element) {
        if (element == nullptr) throw ::jxx::lang::NullPointerException();
    }

    ::jxx::lang::jint compare_(const ::jxx::Ptr<E>& first,
        const ::jxx::Ptr<E>& second) const {
        auto delayed = ::jxx::CAST<Delayed>(first);
        auto other = ::jxx::CAST<Delayed>(second);
        if (delayed == nullptr || other == nullptr) throw ::jxx::lang::ClassCastException();
        return delayed->compareTo(other);
    }

    ::jxx::lang::jlong delay_(const ::jxx::Ptr<E>& element) const {
        return ::jxx::CAST<Delayed>(element)->getDelay(TimeUnit::NANOSECONDS());
    }

    mutable std::mutex mutex_;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    std::deque<::jxx::Ptr<E>> queue_;
};

} // namespace jxx::util::concurrent
