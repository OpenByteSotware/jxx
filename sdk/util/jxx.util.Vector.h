#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.util.AbstractCollection.h"
#include "jxx.util.Iterator.h"
#include "jxx.util.ElementEquals.h"

#include <vector>
#include <stdexcept>

namespace jxx::util {

// Java 8-like Vector<E> (synchronized). Minimal but semantically aligned.
// - Uses Object::synchronized() for monitor semantics (recursive, re-entrant).
// - Provides common methods used by transpiled code: size, isEmpty, add/addElement,
//   elementAt, get, set, remove, removeElementAt, clear, contains, indexOf, lastIndexOf,
//   iterator.
//
// Note: Java Vector iterators are fail-fast; we provide snapshot iteration for simplicity.

template <class E>
class Vector final : public jxx::lang::Object, public AbstractCollection<E> {
public:
    Vector() = default;
    explicit Vector(jint initialCapacity) {
        if (initialCapacity < 0) throw std::invalid_argument("IllegalArgumentException: negative capacity");
        data_.reserve((std::size_t)initialCapacity);
    }

    // ----- Collection interface -----
    jint size() const override {
        return this->synchronized([&]() -> jint { return (jint)data_.size(); });
    }

    jxx::Ptr<Iterator<E>> iterator() override {
        return this->synchronized([&]() -> jxx::Ptr<Iterator<E>> {
            std::vector<E> snap = data_;
            return std::make_shared<SnapshotIt>(std::move(snap));
        });
    }

    // Override add/remove/clear for efficiency and to be synchronized
    jbool add(const E& e) override {
        return this->synchronized([&]() -> jbool {
            data_.push_back(e);
            return true;
        });
    }

    jbool remove(const E& e) override {
        return this->synchronized([&]() -> jbool {
            for (std::size_t i = 0; i < data_.size(); ++i) {
                if (ElementEquals<E>::eq(data_[i], e)) {
                    data_.erase(data_.begin() + (std::ptrdiff_t)i);
                    return true;
                }
            }
            return false;
        });
    }

    void clear() override {
        this->synchronized([&]() {
            data_.clear();
        });
    }

    jbool contains(const E& e) const override {
        return this->synchronized([&]() -> jbool {
            for (const auto& x : data_) {
                if (ElementEquals<E>::eq(x, e)) return true;
            }
            return false;
        });
    }

    // ----- Vector-specific Java APIs -----

    void addElement(const E& e) { (void)add(e); }

    E elementAt(jint index) const {
        return this->synchronized([&]() -> E {
            if (index < 0 || (std::size_t)index >= data_.size()) throw std::out_of_range("ArrayIndexOutOfBoundsException");
            return data_[(std::size_t)index];
        });
    }

    E get(jint index) const { return elementAt(index); }

    E set(jint index, const E& element) {
        return this->synchronized([&]() -> E {
            if (index < 0 || (std::size_t)index >= data_.size()) throw std::out_of_range("ArrayIndexOutOfBoundsException");
            E old = data_[(std::size_t)index];
            data_[(std::size_t)index] = element;
            return old;
        });
    }

    void setElementAt(const E& element, jint index) { (void)set(index, element); }

    void removeElementAt(jint index) {
        this->synchronized([&]() {
            if (index < 0 || (std::size_t)index >= data_.size()) throw std::out_of_range("ArrayIndexOutOfBoundsException");
            data_.erase(data_.begin() + (std::ptrdiff_t)index);
        });
    }

    jbool removeElement(const E& obj) { return remove(obj); }

    jint indexOf(const E& obj) const { return indexOf(obj, 0); }

    jint indexOf(const E& obj, jint fromIndex) const {
        return this->synchronized([&]() -> jint {
            if (fromIndex < 0) fromIndex = 0;
            for (std::size_t i = (std::size_t)fromIndex; i < data_.size(); ++i) {
                if (ElementEquals<E>::eq(data_[i], obj)) return (jint)i;
            }
            return -1;
        });
    }

    jint lastIndexOf(const E& obj) const { return lastIndexOf(obj, (jint)data_.size() - 1); }

    jint lastIndexOf(const E& obj, jint fromIndex) const {
        return this->synchronized([&]() -> jint {
            if (data_.empty()) return -1;
            if (fromIndex >= (jint)data_.size()) fromIndex = (jint)data_.size() - 1;
            for (jint i = fromIndex; i >= 0; --i) {
                if (ElementEquals<E>::eq(data_[(std::size_t)i], obj)) return i;
            }
            return -1;
        });
    }

    void ensureCapacity(jint minCapacity) {
        this->synchronized([&]() {
            if (minCapacity > 0) data_.reserve((std::size_t)minCapacity);
        });
    }

    jint capacity() const {
        return this->synchronized([&]() -> jint { return (jint)data_.capacity(); });
    }

private:
    std::vector<E> data_{};

    class SnapshotIt final : public Iterator<E> {
    public:
        explicit SnapshotIt(std::vector<E> items) : items_(std::move(items)) {}
        jbool hasNext() override { return idx_ < items_.size(); }
        E next() override {
            if (!hasNext()) throw std::out_of_range("NoSuchElementException");
            return items_[idx_++];
        }
        void remove() override {
            throw std::logic_error("UnsupportedOperationException: Iterator.remove()");
        }
    private:
        std::vector<E> items_;
        std::size_t idx_ = 0;
    };
};

} // namespace jxx::util
