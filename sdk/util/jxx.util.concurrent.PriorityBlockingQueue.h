#pragma once

#include <condition_variable>
#include <cstdint>
#include <deque>
#include <mutex>
#include <limits>
#include <algorithm>
#include "lang/jxx.lang.Comparable.h"
#include "util/jxx.util.Comparator.h"
#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.AbstractQueue.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.concurrent.BlockingQueue.h"

namespace jxx::util::concurrent {

template<typename E>
class PriorityBlockingQueue final
    : public ::jxx::lang::ClassBase<
          PriorityBlockingQueue<E>,
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
        PriorityBlockingQueue<E>, JxxSuper, BlockingQueue<E>,
        ::jxx::io::SerializableI>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {
        return JxxClassInfoMarker::Class();
    }

    PriorityBlockingQueue()
        : PriorityBlockingQueue(11, nullptr) {}

    explicit PriorityBlockingQueue(::jxx::lang::jint initialCapacity)
        : PriorityBlockingQueue(initialCapacity, nullptr) {}

    PriorityBlockingQueue(
        ::jxx::lang::jint initialCapacity,
        const ::jxx::Ptr<::jxx::util::Comparator<E>>& comparator)
        : Super(), comparator_(comparator) {
        if (initialCapacity < 1) throw ::jxx::lang::IllegalArgumentException();
    }

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
        if (queue_.empty()) return nullptr;
        auto result = queue_.front();
        queue_.pop_front();
        notFull_.notify_one();
        return result;
    }

    ::jxx::Ptr<E> take() override {
        std::unique_lock<std::mutex> lock(mutex_);
        notEmpty_.wait(lock, [&] { return !queue_.empty(); });
        auto result = queue_.front();
        queue_.pop_front();
        notFull_.notify_one();
        return result;
    }

    ::jxx::Ptr<E> poll(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) override {
        if (unit == nullptr) throw ::jxx::lang::NullPointerException();
        std::unique_lock<std::mutex> lock(mutex_);
        if (!notEmpty_.wait_for(lock, unit->toChrono(timeout), [&] {
                return !queue_.empty();
            })) return nullptr;
        auto result = queue_.front();
        queue_.pop_front();
        notFull_.notify_one();
        return result;
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
        while (!queue_.empty() && count < maximumElements) {
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

    ::jxx::lang::jint compare_(
        const ::jxx::Ptr<E>& first,
        const ::jxx::Ptr<E>& second) const {
        if (comparator_ != nullptr) return comparator_->compare(first, second);
        auto comparable = ::jxx::CAST<::jxx::lang::Comparable<E>>(first);
        if (comparable == nullptr) throw ::jxx::lang::ClassCastException();
        return comparable->compareTo(second);
    }

    ::jxx::Ptr<::jxx::util::Comparator<E>> comparator_;
    mutable std::mutex mutex_;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    std::deque<::jxx::Ptr<E>> queue_;
};

} // namespace jxx::util::concurrent
