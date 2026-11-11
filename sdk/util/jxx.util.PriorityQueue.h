#pragma once

#include <algorithm>
#include <cstddef>
#include <vector>

#include "io/jxx.io.InvalidObjectException.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassCastException.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.AbstractQueue.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.Comparator.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.SortedSet.h"
#include "util/jxx.util.wildcard.CollectionExtends.h"

namespace jxx::util {

template <typename E>
class PriorityQueue final
    : public ::jxx::lang::ClassBase<
          PriorityQueue<E>,
          AbstractQueue<E>,
          ::jxx::io::SerializableI> {
public:
    using JxxSuper = AbstractQueue<E>;
    using Super = ::jxx::lang::ClassBase<
        PriorityQueue<E>, JxxSuper, ::jxx::io::SerializableI>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    static constexpr ::jxx::lang::jint DEFAULT_INITIAL_CAPACITY = 11;

private:
    std::vector<::jxx::Ptr<E>> queue_;
    ::jxx::Ptr<Comparator<E>> comparator_;
    ::jxx::lang::jint modCount_ = 0;

    ::jxx::lang::jint compare_(
        const ::jxx::Ptr<E>& left,
        const ::jxx::Ptr<E>& right) const {
        if (comparator_ != nullptr) {
            return comparator_->compare(left, right);
        }
        const auto comparable = ::jxx::CAST<::jxx::lang::Comparable<E>>(left);
        if (comparable == nullptr) {
            throw ::jxx::lang::ClassCastException();
        }
        return comparable->compareTo(right);
    }

    void siftUp_(std::size_t index) {
        auto value = queue_[index];
        while (index > 0U) {
            const auto parent = (index - 1U) / 2U;
            if (compare_(value, queue_[parent]) >= 0) break;
            queue_[index] = queue_[parent];
            index = parent;
        }
        queue_[index] = value;
    }

    void siftDown_(std::size_t index) {
        const auto size = queue_.size();
        auto value = queue_[index];
        const auto half = size / 2U;
        while (index < half) {
            auto child = index * 2U + 1U;
            if (child + 1U < size &&
                compare_(queue_[child + 1U], queue_[child]) < 0) {
                ++child;
            }
            if (compare_(value, queue_[child]) <= 0) break;
            queue_[index] = queue_[child];
            index = child;
        }
        queue_[index] = value;
    }

    void heapify_() {
        if (queue_.size() < 2U) return;
        for (std::size_t i = queue_.size() / 2U; i > 0U; --i) {
            siftDown_(i - 1U);
        }
    }

    ::jxx::lang::jbool removeAt_(std::size_t index) {
        if (index >= queue_.size()) return false;
        ++modCount_;
        const auto last = queue_.back();
        queue_.pop_back();
        if (index < queue_.size()) {
            queue_[index] = last;
            if (index > 0U &&
                compare_(queue_[index], queue_[(index - 1U) / 2U]) < 0) {
                siftUp_(index);
            } else {
                siftDown_(index);
            }
        }
        return true;
    }

    class PriorityQueueIterator final
        : public ::jxx::lang::ClassBase<
              PriorityQueueIterator,
              ::jxx::lang::Object,
              Iterator<E>> {
        ::jxx::Ptr<PriorityQueue<E>> owner_;
        std::size_t cursor_ = 0U;
        std::size_t lastReturned_ = 0U;
        ::jxx::lang::jint expectedModCount_ = 0;
        ::jxx::lang::jbool canRemove_ = false;

        void check_() const {
            if (owner_->modCount_ != expectedModCount_) {
                throw ConcurrentModificationException();
            }
        }

    public:
        explicit PriorityQueueIterator(
            const ::jxx::Ptr<PriorityQueue<E>>& owner)
            : owner_(owner), expectedModCount_(owner->modCount_) {}

        ::jxx::lang::jbool hasNext() override {
            return cursor_ < owner_->queue_.size();
        }

        ::jxx::Ptr<E> next() override {
            check_();
            if (!hasNext()) throw NoSuchElementException();
            lastReturned_ = cursor_++;
            canRemove_ = true;
            return owner_->queue_[lastReturned_];
        }

        void remove() override {
            check_();
            if (!canRemove_) throw ::jxx::lang::IllegalStateException();
            owner_->removeAt_(lastReturned_);
            cursor_ = lastReturned_;
            expectedModCount_ = owner_->modCount_;
            canRemove_ = false;
        }
    };

public:
    PriorityQueue() { queue_.reserve(DEFAULT_INITIAL_CAPACITY); }

    explicit PriorityQueue(::jxx::lang::jint initialCapacity) {
        if (initialCapacity < 1) throw ::jxx::lang::IllegalArgumentException();
        queue_.reserve(static_cast<std::size_t>(initialCapacity));
    }

    explicit PriorityQueue(const ::jxx::Ptr<Comparator<E>>& comparator)
        : comparator_(comparator) {
        queue_.reserve(DEFAULT_INITIAL_CAPACITY);
    }

    PriorityQueue(
        ::jxx::lang::jint initialCapacity,
        const ::jxx::Ptr<Comparator<E>>& comparator)
        : comparator_(comparator) {
        if (initialCapacity < 1) throw ::jxx::lang::IllegalArgumentException();
        queue_.reserve(static_cast<std::size_t>(initialCapacity));
    }

    explicit PriorityQueue(
        const ::jxx::Ptr<wildcard::CollectionExtends<E>>& collection) {
        if (collection == nullptr) throw ::jxx::lang::NullPointerException();
        const auto iteratorValue = collection->iteratorExtends();
        while (iteratorValue->hasNext()) {
            const auto value = iteratorValue->next();
            if (value == nullptr) throw ::jxx::lang::NullPointerException();
            queue_.push_back(value);
        }
        heapify_();
    }

    explicit PriorityQueue(const ::jxx::Ptr<PriorityQueue<E>>& queue) {
        if (queue == nullptr) throw ::jxx::lang::NullPointerException();
        queue_ = queue->queue_;
        comparator_ = queue->comparator_;
    }

    explicit PriorityQueue(const ::jxx::Ptr<SortedSet<E>>& set) {
        if (set == nullptr) throw ::jxx::lang::NullPointerException();
        comparator_ = set->comparator();
        const auto iteratorValue = set->iterator();
        while (iteratorValue->hasNext()) {
            const auto value = iteratorValue->next();
            if (value == nullptr) throw ::jxx::lang::NullPointerException();
            queue_.push_back(value);
        }
        heapify_();
    }

    ::jxx::lang::jbool offer(const ::jxx::Ptr<E>& element) override {
        if (element == nullptr) throw ::jxx::lang::NullPointerException();
        queue_.push_back(element);
        try {
            siftUp_(queue_.size() - 1U);
        } catch (...) {
            queue_.pop_back();
            throw;
        }
        ++modCount_;
        return true;
    }

    ::jxx::Ptr<E> poll() override {
        if (queue_.empty()) return nullptr;
        const auto result = queue_.front();
        removeAt_(0U);
        return result;
    }

    ::jxx::Ptr<E> peek() override {
        return queue_.empty() ? nullptr : queue_.front();
    }

    ::jxx::lang::jint size() override {
        return static_cast<::jxx::lang::jint>(queue_.size());
    }

    void clear() override {
        if (!queue_.empty()) {
            queue_.clear();
            ++modCount_;
        }
    }

    ::jxx::lang::jbool contains(
        const ::jxx::Ptr<::jxx::lang::Object>& object) override {
        for (const auto& value : queue_) {
            if (value->equals(object)) return true;
        }
        return false;
    }

    ::jxx::lang::jbool remove(
        const ::jxx::Ptr<::jxx::lang::Object>& object) override {
        for (std::size_t i = 0U; i < queue_.size(); ++i) {
            if (queue_[i]->equals(object)) return removeAt_(i);
        }
        return false;
    }

    ::jxx::Ptr<Iterator<E>> iterator() override {
        return ::jxx::NEW<PriorityQueueIterator>(
            ::jxx::CAST<PriorityQueue<E>>(this->thisPtr()));
    }

    ::jxx::Ptr<Comparator<E>> comparator() const { return comparator_; }

    void writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) override {
        if (output == nullptr) throw ::jxx::lang::NullPointerException();
        output->writeObject(::jxx::CAST<::jxx::lang::Object>(comparator_));
        output->writeInt(static_cast<::jxx::lang::jint>(queue_.size()));
        const auto expectedModCount = modCount_;
        for (const auto& value : queue_) {
            output->writeObject(::jxx::CAST<::jxx::lang::Object>(value));
        }
        if (expectedModCount != modCount_) throw ConcurrentModificationException();
    }

    void readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) override {
        if (input == nullptr) throw ::jxx::lang::NullPointerException();
        const auto comparatorObject = input->readObject();
        comparator_ = comparatorObject == nullptr
            ? nullptr
            : ::jxx::CAST<Comparator<E>>(comparatorObject);
        if (comparatorObject != nullptr && comparator_ == nullptr) {
            throw ::jxx::io::InvalidObjectException(
                "PriorityQueue comparator has incompatible type");
        }
        const auto restoredSize = input->readInt();
        if (restoredSize < 0) {
            throw ::jxx::io::InvalidObjectException("Negative PriorityQueue size");
        }
        std::vector<::jxx::Ptr<E>> restored;
        restored.reserve(static_cast<std::size_t>(restoredSize));
        for (::jxx::lang::jint i = 0; i < restoredSize; ++i) {
            const auto object = input->readObject();
            const auto value = object == nullptr ? nullptr : ::jxx::CAST<E>(object);
            if (value == nullptr) {
                throw ::jxx::io::InvalidObjectException(
                    "PriorityQueue contains null or incompatible element");
            }
            restored.push_back(value);
        }
        queue_.swap(restored);
        modCount_ = 0;
        heapify_();
    }

    void readObjectNoData() override {
        queue_.clear();
        comparator_.reset();
        modCount_ = 0;
    }
};

} // namespace jxx::util
