#pragma once
#include <vector>
#include <mutex>
#include <stdexcept>
#include <cstddef>
#include <initializer_list>
#include <algorithm>
#include <utility>

namespace jxx::util {

template <typename T>
class Vector {
public:
    // ---- Constructors ----
    // Java default capacity is 10; we mimic that for initial reserve.
    explicit Vector(std::size_t initialCapacity = 10, std::size_t capacityIncrement = 0)
        : capacityIncrement_(capacityIncrement)
    {
        if (initialCapacity > 0) data_.reserve(initialCapacity);
    }

    Vector(std::initializer_list<T> init, std::size_t capacityIncrement = 0)
        : capacityIncrement_(capacityIncrement)
        , data_(init)
    {}

    // ---- Size / capacity ----
    std::size_t size() const noexcept {
        std::lock_guard<std::mutex> lk(m_);
        return data_.size();
    }

    bool isEmpty() const noexcept { return size() == 0; }

    // Capacity in terms of underlying storage
    std::size_t capacity() const noexcept {
        std::lock_guard<std::mutex> lk(m_);
        return data_.capacity();
    }

    // Ensure capacity (does not change size)
    void ensureCapacity(std::size_t minCapacity) {
        std::lock_guard<std::mutex> lk(m_);
        if (data_.capacity() < minCapacity) {
            growLocked(minCapacity);
        }
    }

    // Trim capacity to current size (like Java's trimToSize)
    void trimToSize() {
        std::lock_guard<std::mutex> lk(m_);
        data_.shrink_to_fit();
    }

    // Set logical size; grow with default-inserted elements or shrink by erasing
    void setSize(std::size_t newSize) {
        std::lock_guard<std::mutex> lk(m_);
        if (newSize > data_.size()) {
            if (newSize > data_.capacity()) {
                growLocked(newSize);
            }
            data_.resize(newSize); // value-initialized T()
        } else if (newSize < data_.size()) {
            data_.erase(data_.begin() + static_cast<long>(newSize), data_.end());
        }
        ++modCount_;
    }

    // ---- Access (by value for thread-safety) ----
    // get(index) returns a copy of element (Java returns reference to object; copying is safest in C++).
    T get(std::size_t index) const {
        std::lock_guard<std::mutex> lk(m_);
        checkIndexLocked(index);
        return data_[index];
    }

    // set(index, value) returns the old element by value
    T set(std::size_t index, const T& value) {
        std::lock_guard<std::mutex> lk(m_);
        checkIndexLocked(index);
        T old = data_[index];
        data_[index] = value;
        ++modCount_;
        return old;
    }

    T set(std::size_t index, T&& value) {
        std::lock_guard<std::mutex> lk(m_);
        checkIndexLocked(index);
        T old = std::move(data_[index]);
        data_[index] = std::move(value);
        ++modCount_;
        return old;
    }

    // firstElement / lastElement (throw if empty)
    T firstElement() const {
        std::lock_guard<std::mutex> lk(m_);
        if (data_.empty()) throw std::out_of_range("Vector.firstElement: empty");
        return data_.front();
    }

    T lastElement() const {
        std::lock_guard<std::mutex> lk(m_);
        if (data_.empty()) throw std::out_of_range("Vector.lastElement: empty");
        return data_.back();
    }

    // ---- Add / Insert ----
    // add(element) -> push_back
    void add(const T& value) {
        std::lock_guard<std::mutex> lk(m_);
        ensureCapacityForPushLocked();
        data_.push_back(value);
        ++modCount_;
    }

    void add(T&& value) {
        std::lock_guard<std::mutex> lk(m_);
        ensureCapacityForPushLocked();
        data_.push_back(std::move(value));
        ++modCount_;
    }

    // add(index, element)
    void add(std::size_t index, const T& value) {
        std::lock_guard<std::mutex> lk(m_);
        checkIndexForInsertLocked(index);
        ensureCapacityForPushLocked();
        data_.insert(data_.begin() + static_cast<long>(index), value);
        ++modCount_;
    }

    void add(std::size_t index, T&& value) {
        std::lock_guard<std::mutex> lk(m_);
        checkIndexForInsertLocked(index);
        ensureCapacityForPushLocked();
        data_.insert(data_.begin() + static_cast<long>(index), std::move(value));
        ++modCount_;
    }

    // ---- Remove ----
    // removeAt(index) -> returns removed element
    T removeAt(std::size_t index) {
        std::lock_guard<std::mutex> lk(m_);
        checkIndexLocked(index);
        auto it = data_.begin() + static_cast<long>(index);
        T removed = std::move(*it);
        data_.erase(it);
        ++modCount_;
        return removed;
    }

    // remove(element) -> remove first occurrence, return true if removed
    bool remove(const T& value) {
        std::lock_guard<std::mutex> lk(m_);
        auto it = std::find(data_.begin(), data_.end(), value);
        if (it == data_.end()) return false;
        data_.erase(it);
        ++modCount_;
        return true;
    }

    void clear() {
        std::lock_guard<std::mutex> lk(m_);
        data_.clear();
        ++modCount_;
    }

    // ---- Search ----
    // indexOf(value, fromIndex=0), returns size_t(-1) if not found (Java returns -1)
    std::size_t indexOf(const T& value, std::size_t fromIndex = 0) const {
        std::lock_guard<std::mutex> lk(m_);
        if (fromIndex > data_.size()) return static_cast<std::size_t>(-1);
        auto it = std::find(data_.begin() + static_cast<long>(fromIndex), data_.end(), value);
        return (it == data_.end()) ? static_cast<std::size_t>(-1)
                                   : static_cast<std::size_t>(std::distance(data_.begin(), it));
    }

    std::size_t lastIndexOf(const T& value) const {
        std::lock_guard<std::mutex> lk(m_);
        auto rit = std::find(data_.rbegin(), data_.rend(), value);
        return (rit == data_.rend()) ? static_cast<std::size_t>(-1)
                                     : static_cast<std::size_t>(std::distance(data_.begin(), rit.base() - 1));
    }

    // ---- Snapshot iteration ----
    // Returns a copy of the current contents; iterate without holding locks.
    std::vector<T> snapshot() const {
        std::lock_guard<std::mutex> lk(m_);
        return data_;
    }

    // Expose a copy for interop with APIs expecting std::vector<T>
    std::vector<T> toStdVector() const { return snapshot(); }

private:
    // ---- Growth / checks (must be called with m_ held) ----
    void ensureCapacityForPushLocked() {
        const std::size_t minCap = data_.size() + 1;
        if (data_.capacity() < minCap) {
            growLocked(minCap);
        }
    }

    void growLocked(std::size_t minCapacity) {
        std::size_t oldCap = data_.capacity();
        std::size_t newCap = oldCap;

        if (oldCap == 0) {
            newCap = (minCapacity > 0) ? minCapacity : std::size_t(1);
        } else if (capacityIncrement_ > 0) {
            // Increase by fixed capacityIncrement until >= minCapacity
            do { newCap += capacityIncrement_; } while (newCap < minCapacity);
        } else {
            // Double until >= minCapacity
            do {
                newCap = (newCap == 0) ? 1 : newCap * 2;
            } while (newCap < minCapacity);
        }
        data_.reserve(newCap);
    }

    void checkIndexLocked(std::size_t index) const {
        if (index >= data_.size()) {
            throw std::out_of_range("Vector index out of range");
        }
    }

    void checkIndexForInsertLocked(std::size_t index) const {
        if (index > data_.size()) {
            throw std::out_of_range("Vector insert index out of range");
        }
    }

private:
    std::vector<T> data_;
    std::size_t capacityIncrement_{0}; // 0 => doubling strategy
    mutable std::mutex m_;
    std::size_t modCount_{0}; // reserved for potential fail-fast iterators
};

} // namespace jxx