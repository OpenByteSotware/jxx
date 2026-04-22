
#pragma once
#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>
#include "lang/jxx_types.h"
#include "lang/jxx.lang.ByteType.h"

using namespace std;

namespace jxx::lang {

    // Forward declaration
    template <typename T, std::uint32_t Rank>
    class JArray;

    // ---------- Init-list metatype to nest initializer_list like Java literals ----------
    template <typename T, std::uint32_t Rank>
    struct JInitList {
        using type = std::initializer_list<typename JInitList<T, Rank - 1>::type>;
    };
    template <typename T>
    struct JInitList<T, 1> {
        using type = std::initializer_list<T>;
    };

    // ======================================================================
// Rank-1 specialization: T[]
// ======================================================================
    template <typename T>
    class JArray<T, 1> {
    public:
        using value_type = T;
        using size_type = std::uint32_t;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using iterator = T*;
        using const_iterator = const T*;

        // Public attribute like Java: arr.length
        size_type length = 0;

        // ---- ctors ----
        JArray() = default;

        explicit JArray(size_type n) : length(n), capacity_(n), data_(n ? std::make_unique<T[]>(n) : std::unique_ptr<T[]>{}) {}


            // From initializer-list: JArray<int,1> a{1,2,3};
            JArray(std::initializer_list<T> init) : JArray(init.size()) {
                std::copy(init.begin(), init.end(), data_.get());
            }

            // Copy
            JArray(const JArray & other) : length(other.length), capacity_(other.length) {
                if (capacity_) {
                    data_.reset(new T[capacity_]);
                    std::copy_n(other.data_.get(), length, data_.get());
                }
            }
            JArray& operator=(const JArray & other) {
                if (this == &other) return *this;
                JArray tmp(other);
                swap(tmp);
                return *this;
            }

            // Move
            JArray(JArray && other) noexcept
                : length(other.length), capacity_(other.capacity_), data_(std::move(other.data_)) {
                other.length = 0;
                other.capacity_ = 0;
            }
            JArray& operator=(JArray && other) noexcept {
                if (this == &other) return *this;
                length = other.length;
                capacity_ = other.capacity_;
                data_ = std::move(other.data_);
                other.length = 0;
                other.capacity_ = 0;
                return *this;
            }

            // ---- element access ----
            reference operator[](size_type i) noexcept { return data_[i]; }      // unchecked (Java-like)
            const_reference operator[](size_type i) const noexcept { return data_[i]; }

            reference at(size_type i) {
                if (i >= length) throw std::out_of_range("JArray<T,1>::at index out of range");
                return data_[i];
            }
            const_reference at(size_type i) const {
                if (i >= length) throw std::out_of_range("JArray<T,1>::at index out of range");
                return data_[i];
            }

            // ---- raw data ----
            pointer data() noexcept { return data_.get(); }
            const_pointer data() const noexcept { return data_.get(); }

            // ---- iterators ----
            iterator begin() noexcept { return data_.get(); }
            const_iterator begin() const noexcept { return data_.get(); }
            const_iterator cbegin() const noexcept { return data_.get(); }

            iterator end() noexcept { return data_.get() + length; }
            const_iterator end() const noexcept { return data_.get() + length; }
            const_iterator cend() const noexcept { return data_.get() + length; }

            bool empty() const noexcept { return length == 0; }
            size_type size() const noexcept { return length; }
            size_type capacity() const noexcept { return capacity_; }

            // ---- algorithms ----
            void fill(const T & v) { std::fill(begin(), end(), v); }

            std::vector<T> to_vector() const { return std::vector<T>(begin(), end()); }

            // ---- vector-like growth ----

            // Ensure capacity >= n (does not change length)
            void reserve(size_type n) {
                if (n <= capacity_) return;
                grow_to_(n);
            }

            // Append a value
            void push_back(const T & value) {
                ensure_capacity_for_push_();
                data_[length++] = value;
            }
            void push_back(T && value) {
                ensure_capacity_for_push_();
                data_[length++] = std::move(value);
            }

            // Perfect-forwarded construction in-place
            template <class... Args>
            reference emplace_back(Args&&... args) {
                ensure_capacity_for_push_();
                // For trivial T this is equivalent to assignment, but keep general
                new (data_.get() + length) T(std::forward<Args>(args)...);
                return data_[length++]; // return reference to newly appended element
            }
      
            void shrink_to_fit() {
            if (capacity_ == length) return;
            if (length == 0) {
                data_.reset();
                capacity_ = 0;
                return;
            }
            std::unique_ptr<T[]> buf(new T[length]);
            std::copy_n(data_.get(), length, buf.get());
            data_.swap(buf);
            capacity_ = length;
        }

        friend bool operator==(const JArray & a, const JArray & b) {
            if (a.length != b.length) return false;
            for (size_type i = 0; i < a.length; ++i) {
                if (!(a[i] == b[i])) return false;
            }
            return true;
        }
        friend bool operator!=(const JArray & a, const JArray & b) { return !(a == b); }

        void swap(JArray & other) noexcept {
            using std::swap;
            swap(length, other.length);
            swap(capacity_, other.capacity_);
            swap(data_, other.data_);
        }

        // ---- (optional) resize from earlier step, unchanged semantics ----
        void resize(size_type n) { resize(n, T{}); }
        void resize(size_type n, const T & fill_value) {
            if (n == length) return;
            if (n <= capacity_) {
                // Just adjust length and initialize the tail if we're growing
                if (n > length) {
                    for (size_type i = length; i < n; ++i) data_[i] = fill_value;
                }
                length = n;
                return;
            }
            // Need a new buffer
            size_type new_cap = n;
            std::unique_ptr<T[]> buf(new T[new_cap]);      // value-init so Java default is preserved
            std::copy_n(data_.get(), length, buf.get());     // copy overlap
            for (size_type i = length; i < n; ++i) buf[i] = fill_value; // tail
            data_.swap(buf);
            capacity_ = new_cap;
            length = n;
        }

    private:
        std::unique_ptr<T[]> data_{};
        size_type capacity_ = 0;

        // Growth policy: double capacity (min 4) until >= need
        void grow_to_(size_type need) {
            size_type new_cap = capacity_ ? capacity_ : size_type{ 4 };
            while (new_cap < need) new_cap = new_cap < (std::numeric_limits<size_type>::max() / 2)
                ? new_cap * 2
                : need; // clamp
            // No need to zero entire capacity; only elements up to 'length' are valid
            std::unique_ptr<T[]> buf(new T[new_cap]);        // uninitialized beyond 'length'
            // Copy existing elements
            if (length) std::copy_n(data_.get(), length, buf.get());
            data_.swap(buf);
            capacity_ = new_cap;
        }

        void ensure_capacity_for_push_() {
            if (length == capacity_) {
                grow_to_(length ? length + 1 : size_type{ 1 });
            }
        }               
    };

    // ======================================================================
        // Rank-N (N>=2): T[][]...
        // ======================================================================
    template <typename T, std::uint32_t Rank>
    class JArray {
        static_assert(Rank >= 2, "Use JArray<T,1> for rank-1 arrays");

    public:
        using SubArray = JArray<T, Rank - 1>;
        using size_type = std::uint32_t;
        using InitList = typename JInitList<T, Rank>::type;

        // Java-like attribute
        size_type length = 0;

        // ---- ctors ----
        JArray() = default;

        // Create outer dimension with 'n' subarrays (default-constructed subarrays)
        explicit JArray(size_type n) : length(n), elems_(n) {}

        // Rectangular constructor: provide sizes for each dimension
        explicit JArray(const std::array<size_type, Rank>& dims) : length(dims[0]) {
            elems_.reserve(length);
            auto tail = tail_dims_(dims);
            for (size_type i = 0; i < length; ++i) {
                elems_.emplace_back(SubArray(tail)); // recursively construct subarrays
            }
        }

        // From nested initializer-lists (Java-like literals)
        JArray(InitList init) : length(init.size()) {
            elems_.reserve(length);
            for (const auto& sub : init) {
                elems_.emplace_back(SubArray(sub));
            }
        }

        // Copy/move
        JArray(const JArray&) = default;
        JArray& operator=(const JArray&) = default;
        JArray(JArray&&) noexcept = default;
        JArray& operator=(JArray&&) noexcept = default;

        // ---- element access (first dimension) ----
        SubArray& operator[](size_type i) noexcept { return elems_[i]; }
        const SubArray& operator[](size_type i) const noexcept { return elems_[i]; }

        SubArray& at(size_type i) {
            if (i >= length) throw std::out_of_range("JArray<T,N>::at index out of range");
            return elems_[i];
        }
        const SubArray& at(size_type i) const {
            if (i >= length) throw std::out_of_range("JArray<T,N>::at index out of range");
            return elems_[i];
        }

        // ---- iterators ----
        auto begin() noexcept { return elems_.begin(); }
        auto end() noexcept { return elems_.end(); }
        auto begin() const noexcept { return elems_.begin(); }
        auto end() const noexcept { return elems_.end(); }
        auto cbegin() const noexcept { return elems_.cbegin(); }
        auto cend() const noexcept { return elems_.cend(); }

        bool empty() const noexcept { return length == 0; }
        size_type size() const noexcept { return length; }
        size_type capacity() const noexcept { return elems_.capacity(); }

        // ---- raw pointer to first subarray ----
        SubArray* data() noexcept { return elems_.empty() ? nullptr : elems_.data(); }
        const SubArray* data() const noexcept { return elems_.empty() ? nullptr : elems_.data(); }

        // ---- algorithms ----
        void fill(const T& v) {
            for (auto& sub : elems_) sub.fill(v);
        }

        // ---- vector-like API (outer dimension) ----

        // Reserve capacity for outer vector
        void reserve(size_type n) { elems_.reserve(n); }

        // Append an existing subarray
        void push_back(const SubArray& sub) {
            elems_.push_back(sub);
            length = elems_.size();
        }
        void push_back(SubArray&& sub) {
            elems_.push_back(std::move(sub));
            length = elems_.size();
        }

        // Emplace a subarray in-place (forwards to SubArray constructors)
        template <class... Args>
        SubArray& emplace_back(Args&&... args) {
            elems_.emplace_back(std::forward<Args>(args)...);
            length = elems_.size();
            return elems_.back();
        }

        // Convenience: push via nested initializer_list for Rank-1 tail type
        // Works for any Rank >= 2 (the type matches JInitList<T, Rank-1>::type)
        void push_back(typename JInitList<T, Rank - 1>::type init) {
            elems_.emplace_back(SubArray(init));
            length = elems_.size();
        }

        // Deep structural equality
        friend bool operator==(const JArray& a, const JArray& b) {
            if (a.length != b.length) return false;
            for (size_type i = 0; i < a.length; ++i) {
                if (!(a.elems_[i] == b.elems_[i])) return false;
            }
            return true;
        }
        friend bool operator!=(const JArray& a, const JArray& b) { return !(a == b); }

        void swap(JArray& other) noexcept {
            using std::swap;
            swap(length, other.length);
            elems_.swap(other.elems_);
        }

        // (Optional) Resize methods from earlier step (unchanged)
        void resize(size_type n) {
            elems_.resize(n);  // default-construct SubArray()
            length = elems_.size();
        }
        void resize(size_type n, const std::array<size_type, Rank - 1>& tail_dims) {
            const size_type old = elems_.size();
            if (n <= old) {
                elems_.resize(n);
            }
            else {
                elems_.reserve(n);
                for (size_type i = old; i < n; ++i) {
                    elems_.emplace_back(SubArray(tail_dims));
                }
            }
            length = elems_.size();
        }

    private:
        std::vector<SubArray> elems_{};

        static std::array<size_type, Rank - 1>
            tail_dims_(const std::array<size_type, Rank>& dims) {
            std::array<size_type, Rank - 1> tail{};
            for (std::size_t i = 1; i < Rank; ++i) tail[i - 1] = dims[i];
            return tail;
        }
    };
}

using ByteArray = jxx::lang::JArray<jxx::lang::jbyte, 1>;
using CharArray = jxx::lang::JArray<jxx::lang::jchar, 1>;
using IntArray = jxx::lang::JArray<jxx::lang::jint, 1>;


