
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

namespace jxx::lang {

    // Forward declaration
    template <typename T, std::size_t Rank>
    class JArray;

    // ---------- Init-list metatype to nest initializer_list like Java literals ----------
    template <typename T, std::size_t Rank>
    struct JInitList {
        using type = std::initializer_list<typename JInitList<T, Rank - 1>::type>;
    };
    template <typename T>
    struct JInitList<T, 1> {
        using type = std::initializer_list<T>;
    };

    // ==================== Rank-1 specialization: T[] ====================
    template <typename T>
    class JArray<T, 1> {
    public:
        using value_type = T;
        using size_type = std::size_t;
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

        explicit JArray(size_type n)
            : length(n),
            data_(n ? std::make_unique<T[]>(n) : std::unique_ptr<T[]>{}) {}

        // From initializer-list: JArray<int,1> a{1,2,3};
        JArray(std::initializer_list<T> init)
            : JArray(init.size()) {
            std::copy(init.begin(), init.end(), data_.get());
        }

        // Copy
        JArray(const JArray& other)
            : JArray(other.length) {
            if (length) std::copy(other.data_.get(), other.data_.get() + length, data_.get());
        }

        JArray& operator=(const JArray& other) {
            if (this == &other) return *this;
            JArray tmp(other);
            swap(tmp);
            return *this;
        }

        // Move
        JArray(JArray&& other) noexcept
            : length(other.length),
            data_(std::move(other.data_)) {
            other.length = 0;
        }

        JArray& operator=(JArray&& other) noexcept {
            if (this == &other) return *this;
            length = other.length;
            data_ = std::move(other.data_);
            other.length = 0;
            return *this;
        }

        // ---- element access ----
        reference operator[](size_type i) noexcept { return data_[i]; }                  // like Java: unchecked
        const_reference operator[](size_type i) const noexcept { return data_[i]; }

        reference at(size_type i) {
            if (i >= length) throw std::out_of_range("JArray<T,1>::at index out of range");
            return data_[i];
        }
        const_reference at(size_type i) const {
            if (i >= length) throw std::out_of_range("JArray<T,1>::at index out of range");
            return data_[i];
        }


        void resize(size_type n) {
            resize(n, T{});
        }

        // Resize to 'n' and fill any newly added elements with 'fill_value'.
        void resize(size_type n, const T& fill_value) {
            if (n == length) return;

            std::unique_ptr<T[]> new_buf;
            if (n) {
                // value-init the whole buffer so arithmetic types get zero-initialized
                new_buf.reset(new T[n]);
                const size_type m = (n < length) ? n : length;

                // copy the overlap
                for (size_type i = 0; i < m; ++i) {
                    new_buf[i] = std::move_if_noexcept(data_[i]);
                }
                // initialize the tail
                for (size_type i = m; i < n; ++i) {
                    new_buf[i] = fill_value;
                }
            }

            // commit: swap buffers and update length
            data_.swap(new_buf);
            length = n;
        }

            
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
        std::size_t size() const noexcept { return length; }

        // ---- algorithms ----
        void fill(const T& v) {
            std::fill(begin(), end(), v);
        }

        std::vector<T> to_vector() const {
            return std::vector<T>(begin(), end());
        }

        friend bool operator==(const JArray& a, const JArray& b) {
            if (a.length != b.length) return false;
            for (size_type i = 0; i < a.length; ++i) {
                if (!(a[i] == b[i])) return false;
            }
            return true;
        }
        friend bool operator!=(const JArray& a, const JArray& b) { return !(a == b); }

        void swap(JArray& other) noexcept {
            using std::swap;
            swap(length, other.length);
            swap(data_, other.data_);
        }

    private:
        std::unique_ptr<T[]> data_{};
    };

    // ==================== Rank-N (N>=2): T[][]... ====================
    template <typename T, std::size_t Rank>
    class JArray {
        static_assert(Rank >= 2, "Use JArray<T,1> for rank-1 arrays");
    public:
        using SubArray = JArray<T, Rank - 1>;
        using size_type = std::size_t;
        using InitList = typename JInitList<T, Rank>::type;

        // Java-like attribute
        size_type length = 0;

        // ---- ctors ----
        JArray() = default;

        // Create outer dimension with 'n' subarrays (initially empty subarrays of length 0)
        explicit JArray(size_type n)
            : length(n), elems_(n) {}

        // Rectangular constructor: provide sizes for each dimension
        explicit JArray(const std::array<size_type, Rank>& dims)
            : length(dims[0]) {
            elems_.reserve(length);
            auto tail = tail_dims_(dims);
            for (size_type i = 0; i < length; ++i) {
                elems_.emplace_back(SubArray(tail)); // recursively construct subarrays
            }
        }

        // From nested initializer-lists (Java-like literals)
        JArray(InitList init) {
            length = init.size();
            elems_.reserve(length);
            for (const auto& sub : init) {
                elems_.emplace_back(SubArray(sub));
            }
        }

        // Copy/move
        JArray(const JArray& other) = default;
        JArray& operator=(const JArray& other) = default;
        JArray(JArray&& other) noexcept = default;
        JArray& operator=(JArray&& other) noexcept = default;

        // ---- element access (first dimension) ----
        SubArray& operator[](size_type i) noexcept { return elems_[i]; }        // unchecked like Java
        const SubArray& operator[](size_type i) const noexcept { return elems_[i]; }

        SubArray& at(size_type i) {
            if (i >= length) throw std::out_of_range("JArray<T,N>::at index out of range");
            return elems_[i];
        }
        const SubArray& at(size_type i) const {
            if (i >= length) throw std::out_of_range("JArray<T,N>::at index out of range");
            return elems_[i];
        }


        void resize(size_type n) {
            elems_.resize(n);  // default-constructs new SubArray()
            length = n;
        }

        // Resize the outer dimension and construct new subarrays with a rectangular shape
        // defined by 'tail_dims' (sizes for Rank-1..Rank-1 dimensions).
        void resize(size_type n, const std::array<size_type, Rank - 1>& tail_dims) {
            const size_type old = elems_.size();
            if (n <= old) {
                elems_.resize(n);
            }
            else {
                elems_.reserve(n);
                for (size_type i = old; i < n; ++i) {
                    elems_.emplace_back(SubArray(tail_dims)); // recursively constructs leaves
                }
            }
            length = n;
        }


        // Iteration over subarrays (first dimension)
        auto begin() noexcept { return elems_.begin(); }
        auto end() noexcept { return elems_.end(); }
        auto begin() const noexcept { return elems_.begin(); }
        auto end() const noexcept { return elems_.end(); }
        auto cbegin() const noexcept { return elems_.cbegin(); }
        auto cend() const noexcept { return elems_.cend(); }

        bool empty() const noexcept { return length == 0; }
        std::size_t size() const noexcept { return length; }

        // Deep fill leaves with a value (like nested loops setting base type)
        void fill(const T& v) {
            for (auto& sub : elems_) sub.fill(v);
        }

        // Structural equality, deep compare
        friend bool operator==(const JArray& a, const JArray& b) {
            if (a.length != b.length) return false;
            for (size_type i = 0; i < a.length; ++i) {
                if (!(a.elems_[i] == b.elems_[i])) return false;
            }
            return true;
        }
        friend bool operator!=(const JArray& a, const JArray& b) { return !(a == b); }

        void swap(JArray& other) noexcept { elems_.swap(other.elems_); std::swap(length, other.length); }

    private:
        std::vector<SubArray> elems_{};

        static std::array<size_type, Rank - 1> tail_dims_(const std::array<size_type, Rank>& dims) {
            std::array<size_type, Rank - 1> tail{};
            for (std::size_t i = 1; i < Rank; ++i) tail[i - 1] = dims[i];
            return tail;
        }
    }; 
}

using ByteArray = jxx::lang::JArray<jxx::lang::jbyte, 1>;
using CharArray = jxx::lang::JArray<jxx::lang::jchar, 1>;
using IntArray = jxx::lang::JArray<jxx::lang::jint, 1>;
