#pragma once

#include <condition_variable>
#include <cstdint>
#include <deque>
#include <mutex>
#include <limits>
#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Thread.h"
#include "util/jxx.util.AbstractQueue.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.concurrent.TransferQueue.h"

namespace jxx::util::concurrent {

template<typename E>
class LinkedTransferQueue final
    : public ::jxx::lang::ClassBase<
          LinkedTransferQueue<E>,
          ::jxx::util::AbstractQueue<E>,
          TransferQueue<E>,
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
        LinkedTransferQueue<E>, JxxSuper, TransferQueue<E>,
        ::jxx::io::SerializableI>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {
        return JxxClassInfoMarker::Class();
    }

    LinkedTransferQueue()
        : Super(), capacity_(std::numeric_limits<::jxx::lang::jint>::max()) {}

    ::jxx::lang::jbool offer(const ::jxx::Ptr<E>& element) override {
        requireElement_(element);
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.size() >= static_cast<std::size_t>(capacity_)) return false;
        queue_.push_back(element); transferTokens_.push_back(0);
        notEmpty_.notify_one();
        return true;
    }

    void put(const ::jxx::Ptr<E>& element) override {
        if (::jxx::lang::Thread::interrupted()) throw ::jxx::lang::InterruptedException();
        requireElement_(element);
        std::unique_lock<std::mutex> lock(mutex_);
        notFull_.wait(lock, [&] { return queue_.size() < static_cast<std::size_t>(capacity_); });
        queue_.push_back(element); transferTokens_.push_back(0);
        notEmpty_.notify_one();
    }

    ::jxx::lang::jbool offer(
        const ::jxx::Ptr<E>& element,
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) override {
        requireElement_(element);
        if (unit == nullptr) throw ::jxx::lang::NullPointerException();
        std::unique_lock<std::mutex> lock(mutex_);
        if (!notFull_.wait_for(lock, unit->toChrono(timeout), [&] {
                return queue_.size() < static_cast<std::size_t>(capacity_);
            })) return false;
        queue_.push_back(element); transferTokens_.push_back(0);
        notEmpty_.notify_one();
        return true;
    }

    void transfer(const ::jxx::Ptr<E>& element) override {
        if (::jxx::lang::Thread::interrupted()) throw ::jxx::lang::InterruptedException();
        requireElement_(element);
        std::unique_lock<std::mutex> lock(mutex_);
        const auto token=++nextTransferToken_;
        queue_.push_back(element); transferTokens_.push_back(token); notEmpty_.notify_all();
        transferred_.wait(lock,[&]{return consumedTransferToken_>=token;});
    }
    ::jxx::lang::jbool tryTransfer(const ::jxx::Ptr<E>& element) override {
        requireElement_(element); std::unique_lock<std::mutex> lock(mutex_);
        if(waitingConsumers_<=0)return false;
        const auto token=++nextTransferToken_; queue_.push_back(element); transferTokens_.push_back(token); notEmpty_.notify_all();
        transferred_.wait(lock,[&]{return consumedTransferToken_>=token;}); return true;
    }
    ::jxx::lang::jbool tryTransfer(const ::jxx::Ptr<E>& element,::jxx::lang::jlong timeout,const ::jxx::Ptr<TimeUnit>& unit) override {
        requireElement_(element); if(!unit)throw ::jxx::lang::NullPointerException();
        std::unique_lock<std::mutex> lock(mutex_); auto deadline=std::chrono::steady_clock::now()+unit->toChrono(timeout);
        if(!notEmpty_.wait_until(lock,deadline,[&]{return waitingConsumers_>0;}))return false;
        const auto token=++nextTransferToken_; queue_.push_back(element); transferTokens_.push_back(token); notEmpty_.notify_all();
        if(transferred_.wait_until(lock,deadline,[&]{return consumedTransferToken_>=token;}))return true;
        for(std::size_t i=0;i<transferTokens_.size();++i)if(transferTokens_[i]==token){queue_.erase(queue_.begin()+i);transferTokens_.erase(transferTokens_.begin()+i);break;} return false;
    }
    ::jxx::lang::jbool hasWaitingConsumer() override { std::lock_guard<std::mutex> lock(mutex_); return waitingConsumers_>0; }
    ::jxx::lang::jint getWaitingConsumerCount() override { std::lock_guard<std::mutex> lock(mutex_); return waitingConsumers_; }

    ::jxx::Ptr<E> poll() override {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) return nullptr;
        auto result = queue_.front();
        queue_.pop_front();
        auto token=transferTokens_.front(); transferTokens_.pop_front(); if(token){consumedTransferToken_=std::max(consumedTransferToken_,token);transferred_.notify_all();}
        notFull_.notify_one();
        return result;
    }

    ::jxx::Ptr<E> take() override {
        if (::jxx::lang::Thread::interrupted()) throw ::jxx::lang::InterruptedException();
        std::unique_lock<std::mutex> lock(mutex_);
        ++waitingConsumers_;
        notEmpty_.notify_all();
        notEmpty_.wait(lock, [&] { return !queue_.empty(); });
        --waitingConsumers_;
        auto result = queue_.front();
        queue_.pop_front();
        auto token=transferTokens_.front(); transferTokens_.pop_front(); if(token){consumedTransferToken_=std::max(consumedTransferToken_,token);transferred_.notify_all();}
        notFull_.notify_one();
        return result;
    }

    ::jxx::Ptr<E> poll(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) override {
        if (unit == nullptr) throw ::jxx::lang::NullPointerException();
        std::unique_lock<std::mutex> lock(mutex_);
        ++waitingConsumers_;
        notEmpty_.notify_all();
        if (!notEmpty_.wait_for(lock, unit->toChrono(timeout), [&] {
                return !queue_.empty();
            })) { --waitingConsumers_; return nullptr; }
        --waitingConsumers_;
        auto result = queue_.front();
        queue_.pop_front();
        auto token=transferTokens_.front(); transferTokens_.pop_front(); if(token){consumedTransferToken_=std::max(consumedTransferToken_,token);transferred_.notify_all();}
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
        return capacity_ - static_cast<::jxx::lang::jint>(queue_.size());
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
        return drainTo(target, capacity_);
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

    mutable std::mutex mutex_;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    std::condition_variable transferred_;
    std::deque<::jxx::Ptr<E>> queue_;
    std::deque<::jxx::lang::jlong> transferTokens_;
    ::jxx::lang::jint waitingConsumers_ = 0;
    ::jxx::lang::jlong nextTransferToken_ = 0;
    ::jxx::lang::jlong consumedTransferToken_ = 0;
    ::jxx::lang::jint capacity_;
};

} // namespace jxx::util::concurrent
