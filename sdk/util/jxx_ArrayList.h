#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>
#include <utility>
#include <cstddef>
#include <type_traits>
#include <iterator>
#include <functional>

namespace jxx::util {

template <typename T>
class ArrayList {
public:
    // ---- Types ----
    using value_type             = T;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using reference              = T&;
    using const_reference        = const T&;
    using pointer                = T*;
    using const_pointer          = const T*;
    using iterator               = typename std::vector<T>::iterator;
    using const_iterator         = typename std::vector<T>::const_iterator;
    using reverse_iterator       = typename std::vector<T>::reverse_iterator;
    using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;

    static constexpr size_type npos = static_cast<size_type>(-1);

    // ---- Constructors ----
    ArrayList() = default;

    explicit ArrayList(size_type initialCapacity) {
        if (initialCapacity > 0) data_.reserve(initialCapacity);
    }

    ArrayList(std::initializer_list<T> init) : data_(init) {}

    // ---- Size / capacity ----
    size_type size() const noexcept { return data_.size(); }
    bool isEmpty() const noexcept { return data_.empty(); }
    size_type capacity() const noexcept { return data_.capacity(); }

    // Ensure capacity (does not change size)
    void ensureCapacity(size_type minCapacity) {
        if (data_.capacity() < minCapacity) data_.reserve(minCapacity);
    }

    // Trim capacity to current size
    void trimToSize() { data_.shrink_to_fit(); }

    // ---- Element access ----
    // Bounds-checked (throws std::out_of_range on error)
    reference at(size_type index) {
        checkIndex(index);
        return data_[index];
    }
    const_reference at(size_type index) const {
        checkIndex(index);
        return data_[index];
    }

    // Unchecked (like Java's get but without explicit bounds checks)
    reference operator[](size_type index) { return data_[index]; }
    const_reference operator[](size_type index) const { return data_[index]; }

    // Java-like get returns reference (since this is unsynchronized)
    reference get(size_type index) {
        checkIndex(index);
        return data_[index];
    }
    const_reference get(size_type index) const {
        checkIndex(index);
        return data_[index];
    }

    // Java-like set returns the previous element (by value)
    T set(size_type index, const T& value) {
        checkIndex(index);
        T old = data_[index];
        data_[index] = value;
        return old;
    }
    T set(size_type index, T&& value) {
        checkIndex(index);
        T old = std::move(data_[index]);
        data_[index] = std::move(value);
        return old;
    }

    // ---- Add / insert ----
    void add(const T& value) { data_.push_back(value); }
    void add(T&& value)      { data_.push_back(std::move(value)); }

    // add(index, value)
    void add(size_type index, const T& value) {
        checkIndexForInsert(index);
        data_.insert(data_.begin() + static_cast<difference_type>(index), value);
    }
    void add(size_type index, T&& value) {
        checkIndexForInsert(index);
        data_.insert(data_.begin() + static_cast<difference_type>(index), std::move(value));
    }

    // Emplace conveniences (not in Java, but idiomatic C++)
    template <typename... Args>
    reference emplaceBack(Args&&... args) {
        data_.emplace_back(std::forward<Args>(args)...);
        return data_.back();
    }
    template <typename... Args>
    reference emplace(size_type index, Args&&... args) {
        checkIndexForInsert(index);
        auto it = data_.emplace(data_.begin() + static_cast<difference_type>(index),
                                std::forward<Args>(args)...);
        return *it;
    }

    // ---- Remove ----
    // removeAt(index) returns removed element (by value)
    T removeAt(size_type index) {
        checkIndex(index);
        auto it = data_.begin() + static_cast<difference_type>(index);
        T removed = std::move(*it);
        data_.erase(it);
        return removed;
    }

    // remove(value): remove first occurrence; return true if removed
    bool remove(const T& value) {
        auto it = std::find(data_.begin(), data_.end(), value);
        if (it == data_.end()) return false;
        data_.erase(it);
        return true;
    }

    // removeIf(predicate): returns true if anything was removed
    template <typename Pred>
    bool removeIf(Pred&& p) {
        auto oldSize = data_.size();
        data_.erase(std::remove_if(data_.begin(), data_.end(), std::forward<Pred>(p)),
                    data_.end());
        return data_.size() != oldSize;
    }

    void clear() noexcept { data_.clear(); }

    // ---- Search / contains ----
    bool contains(const T& value) const {
        return std::find(data_.begin(), data_.end(), value) != data_.end();
    }

    size_type indexOf(const T& value) const {
        auto it = std::find(data_.begin(), data_.end(), value);
        return (it == data_.end()) ? npos : static_cast<size_type>(std::distance(data_.begin(), it));
    }

    size_type lastIndexOf(const T& value) const {
        auto rit = std::find(data_.rbegin(), data_.rend(), value);
        return (rit == data_.rend())
            ? npos
            : static_cast<size_type>(std::distance(data_.begin(), rit.base() - 1));
    }

    // ---- Bulk ops / snapshot ----
    std::vector<T> toVector() const { return data_; }

    // equals (content equality, like Java's List.equals)
    bool equals(const ArrayList& other) const {
        return data_ == other.data_;
    }

    // ---- Sublist ----
    // Returns a copy (like new ArrayList<>(list.subList(...)) in Java)
    ArrayList subList(size_type fromIndex, size_type toIndex) const {
        checkRange(fromIndex, toIndex);
        ArrayList out;
        out.data_.reserve(toIndex - fromIndex);
        out.data_.insert(out.data_.end(),
                         data_.begin() + static_cast<difference_type>(fromIndex),
                         data_.begin() + static_cast<difference_type>(toIndex));
        return out;
    }

    // ---- Sorting ----
    // Natural order (requires operator<)
    void sort() {
        std::sort(data_.begin(), data_.end());
    }
    // Custom comparator: comp(a,b) returns true if a<b
    template <typename Comp>
    void sort(Comp comp) {
        std::sort(data_.begin(), data_.end(), std::move(comp));
    }

    // ---- Iteration (standard C++ iterators) ----
    iterator begin() noexcept { return data_.begin(); }
    iterator end()   noexcept { return data_.end(); }
    const_iterator begin() const noexcept { return data_.begin(); }
    const_iterator end()   const noexcept { return data_.end(); }
    const_iterator cbegin() const noexcept { return data_.cbegin(); }
    const_iterator cend()   const noexcept { return data_.cend(); }

    reverse_iterator rbegin() noexcept { return data_.rbegin(); }
    reverse_iterator rend()   noexcept { return data_.rend(); }
    const_reverse_iterator rbegin() const noexcept { return data_.rbegin(); }
    const_reverse_iterator rend()   const noexcept { return data_.rend(); }

private:
    void checkIndex(size_type index) const {
        if (index >= data_.size()) {
            throw std::out_of_range("ArrayList index out of range");
        }
    }
    void checkIndexForInsert(size_type index) const {
        if (index > data_.size()) {
            throw std::out_of_range("ArrayList insert index out of range");
        }
    }
    void checkRange(size_type from, size_type to) const {
        if (from > to || to > data_.size()) {
            throw std::out_of_range("ArrayList subList range out of range");
        }
    }

private:
    std::vector<T> data_;
};

} // namespace jxx