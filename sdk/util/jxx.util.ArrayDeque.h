#pragma once

#include <deque>

#include "io/jxx.util.AbstractQueue.h"
#include "io/jxx.util.Deque.h"
#include "io/jxx.lang.Cloneable.h"
#include "io/jxx.io.Serializable.h"
#include "io/jxx.lang.Casts.h"
#include "io/jxx.lang.Exceptions.h"

namespace jxx { template <typename T> class Ptr; }

namespace jxx {
namespace util {

template <typename E>
class ArrayDeque
    : public AbstractQueue<E>
    , public virtual Deque<E>
    , public virtual jxx::lang::Cloneable
    , public virtual jxx::io::Serializable {
private:
    std::deque<jxx::Ptr<E>> data_;
    jxx::lang::jint modCount_;

public:
    ArrayDeque()
        : data_()
        , modCount_(0) {
    }

    explicit ArrayDeque(jxx::lang::jint numElements)
        : data_()
        , modCount_(0) {
        if (numElements < 0) throw IllegalArgumentException();
        data_.resize(static_cast<std::size_t>(numElements));
        data_.clear();
    }

    explicit ArrayDeque(jxx::Ptr<wildcard::CollectionExtends<E>> c)
        : data_()
        , modCount_(0) {
        if (c == nullptr) throw NullPointerException();
        this->addAll(c);
    }

    virtual ~ArrayDeque() = default;

    virtual jxx::lang::jint size() override {
        return static_cast<jxx::lang::jint>(data_.size());
    }

    virtual jxx::lang::jbool isEmpty() override {
        return data_.empty();
    }

    virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override {
        for (const auto& e : data_) {
            if (o == nullptr) {
                if (e == nullptr) return true;
            } else if (e != nullptr && o->equals(jxx::lang::ptr_static_cast<jxx::lang::Object>(e))) {
                return true;
            }
        }
        return false;
    }

    class ForwardIterator : public virtual Iterator<E> {
    private:
        ArrayDeque<E>* owner_;
        jxx::lang::jint index_;
        jxx::lang::jint lastRet_;
        jxx::lang::jint expectedModCount_;
    public:
        explicit ForwardIterator(ArrayDeque<E>* owner)
            : owner_(owner), index_(0), lastRet_(-1), expectedModCount_(owner->modCount_) {}
        virtual ~ForwardIterator() = default;

        virtual jxx::lang::jbool hasNext() override {
            return index_ < owner_->size();
        }

        virtual jxx::Ptr<E> next() override {
            checkForComodification();
            if (index_ >= owner_->size()) throw NoSuchElementException();
            lastRet_ = index_;
            return owner_->data_[index_++];
        }

        virtual void remove() override {
            if (lastRet_ < 0) throw IllegalStateException();
            checkForComodification();
            owner_->eraseAt(lastRet_);
            if (lastRet_ < index_) --index_;
            lastRet_ = -1;
            expectedModCount_ = owner_->modCount_;
        }
    private:
        void checkForComodification() {
            if (owner_->modCount_ != expectedModCount_) throw ConcurrentModificationException();
        }
    };

    class DescendingIteratorImpl : public virtual Iterator<E> {
    private:
        ArrayDeque<E>* owner_;
        jxx::lang::jint index_;
        jxx::lang::jint lastRet_;
        jxx::lang::jint expectedModCount_;
    public:
        explicit DescendingIteratorImpl(ArrayDeque<E>* owner)
            : owner_(owner), index_(owner->size() - 1), lastRet_(-1), expectedModCount_(owner->modCount_) {}
        virtual ~DescendingIteratorImpl() = default;

        virtual jxx::lang::jbool hasNext() override {
            return index_ >= 0;
        }

        virtual jxx::Ptr<E> next() override {
            checkForComodification();
            if (index_ < 0) throw NoSuchElementException();
            lastRet_ = index_;
            return owner_->data_[index_--];
        }

        virtual void remove() override {
            if (lastRet_ < 0) throw IllegalStateException();
            checkForComodification();
            owner_->eraseAt(lastRet_);
            if (lastRet_ <= index_) ++index_;
            lastRet_ = -1;
            expectedModCount_ = owner_->modCount_;
        }
    private:
        void checkForComodification() {
            if (owner_->modCount_ != expectedModCount_) throw ConcurrentModificationException();
        }
    };

    virtual jxx::Ptr<Iterator<E>> iterator() override {
        return jxx::Ptr<Iterator<E>>(new ForwardIterator(this));
    }

    virtual jxx::Ptr<Iterator<E>> descendingIterator() override {
        return jxx::Ptr<Iterator<E>>(new DescendingIteratorImpl(this));
    }

    virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>>> toArray() override {
        auto result = jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>>>(new JxxArray<jxx::Ptr<jxx::lang::Object>>(size()));
        for (jxx::lang::jint i = 0; i < size(); ++i) {
            result->set(i, jxx::lang::ptr_static_cast<jxx::lang::Object>(data_[static_cast<std::size_t>(i)]));
        }
        return result;
    }

    virtual jxx::lang::jbool offer(jxx::Ptr<E> e) override {
        return offerLast(e);
    }

    virtual jxx::lang::jbool offerFirst(jxx::Ptr<E> e) override {
        if (e == nullptr) throw NullPointerException();
        data_.push_front(e);
        ++modCount_;
        return true;
    }

    virtual jxx::lang::jbool offerLast(jxx::Ptr<E> e) override {
        if (e == nullptr) throw NullPointerException();
        data_.push_back(e);
        ++modCount_;
        return true;
    }

    virtual void addFirst(jxx::Ptr<E> e) override {
        if (!offerFirst(e)) throw IllegalStateException();
    }

    virtual void addLast(jxx::Ptr<E> e) override {
        if (!offerLast(e)) throw IllegalStateException();
    }

    virtual jxx::lang::jbool add(jxx::Ptr<E> e) override {
        return offerLast(e);
    }

    virtual jxx::Ptr<E> poll() override {
        return pollFirst();
    }

    virtual jxx::Ptr<E> remove() override {
        return removeFirst();
    }

    virtual jxx::Ptr<E> peek() override {
        return peekFirst();
    }

    virtual jxx::Ptr<E> element() override {
        return getFirst();
    }

    virtual jxx::Ptr<E> removeFirst() override {
        auto x = pollFirst();
        if (x != nullptr) return x;
        throw NoSuchElementException();
    }

    virtual jxx::Ptr<E> removeLast() override {
        auto x = pollLast();
        if (x != nullptr) return x;
        throw NoSuchElementException();
    }

    virtual jxx::Ptr<E> pollFirst() override {
        if (data_.empty()) return nullptr;
        auto x = data_.front();
        data_.pop_front();
        ++modCount_;
        return x;
    }

    virtual jxx::Ptr<E> pollLast() override {
        if (data_.empty()) return nullptr;
        auto x = data_.back();
        data_.pop_back();
        ++modCount_;
        return x;
    }

    virtual jxx::Ptr<E> getFirst() override {
        auto x = peekFirst();
        if (x != nullptr) return x;
        throw NoSuchElementException();
    }

    virtual jxx::Ptr<E> getLast() override {
        auto x = peekLast();
        if (x != nullptr) return x;
        throw NoSuchElementException();
    }

    virtual jxx::Ptr<E> peekFirst() override {
        return data_.empty() ? nullptr : data_.front();
    }

    virtual jxx::Ptr<E> peekLast() override {
        return data_.empty() ? nullptr : data_.back();
    }

    virtual jxx::lang::jbool removeFirstOccurrence(jxx::Ptr<jxx::lang::Object> o) override {
        for (jxx::lang::jint i = 0; i < size(); ++i) {
            auto& e = data_[static_cast<std::size_t>(i)];
            if (o == nullptr) {
                if (e == nullptr) {
                    eraseAt(i);
                    return true;
                }
            } else if (e != nullptr && o->equals(jxx::lang::ptr_static_cast<jxx::lang::Object>(e))) {
                eraseAt(i);
                return true;
            }
        }
        return false;
    }

    virtual jxx::lang::jbool removeLastOccurrence(jxx::Ptr<jxx::lang::Object> o) override {
        for (jxx::lang::jint i = size() - 1; i >= 0; --i) {
            auto& e = data_[static_cast<std::size_t>(i)];
            if (o == nullptr) {
                if (e == nullptr) {
                    eraseAt(i);
                    return true;
                }
            } else if (e != nullptr && o->equals(jxx::lang::ptr_static_cast<jxx::lang::Object>(e))) {
                eraseAt(i);
                return true;
            }
        }
        return false;
    }

    virtual void push(jxx::Ptr<E> e) override {
        addFirst(e);
    }

    virtual jxx::Ptr<E> pop() override {
        return removeFirst();
    }

    virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
        return removeFirstOccurrence(o);
    }

    virtual void clear() override {
        if (!data_.empty()) {
            data_.clear();
            ++modCount_;
        }
    }

    virtual jxx::Ptr<jxx::lang::Object> clone() {
        auto cloned = jxx::Ptr<ArrayDeque<E>>(new ArrayDeque<E>());
        for (const auto& e : data_) {
            cloned->data_.push_back(e);
        }
        return cloned;
    }

private:
    void eraseAt(jxx::lang::jint index) {
        data_.erase(data_.begin() + index);
        ++modCount_;
    }
};

} // namespace util
} // namespace jxx
