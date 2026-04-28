#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>
#include <stdexcept>

#include "jxx_types.h"
#include "jxx.lang.ByteType.h"
#include "jxx.lang.String.h"

namespace jxx::lang {

    // Forward declaration
    template <typename T, std::uint32_t Rank>
    class JxxArray;

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
    // Java parity:
    //   - public length field
    //   - operator[] always bounds-checks and throws ArrayIndexOutOfBoundsException
    //   - supports negative indices via jint overload
    // ======================================================================
    template <typename T>
    class JxxArray<T, 1> {
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
        JxxArray() = default;

        explicit JxxArray(size_type n)
            : length(n),
            capacity_(n),
            data_(n ? std::make_unique<T[]>(n) : std::unique_ptr<T[]>{}) {}

        // From initializer-list: JxxArray<int,1> a{1,2,3};
        JxxArray(std::initializer_list<T> init)
            : JxxArray(static_cast<size_type>(init.size())) {
            std::copy(init.begin(), init.end(), data_.get());
        }

        // Copy
        JxxArray(const JxxArray& other)
            : length(other.length),
            capacity_(other.length) {
            if (capacity_) {
                data_.reset(new T[capacity_]);
                std::copy_n(other.data_.get(), length, data_.get());
            }
        }

        JxxArray& operator=(const JxxArray& other) {
            if (this == &other) return *this;
            JxxArray tmp(other);
            swap(tmp);
            return *this;
        }

        // Move
        JxxArray(JxxArray&& other) noexcept
            : length(other.length),
            capacity_(other.capacity_),
            data_(std::move(other.data_)) {
            other.length = 0;
            other.capacity_ = 0;
        }

        JxxArray& operator=(JxxArray&& other) noexcept {
            if (this == &other) return *this;
            length = other.length;
            capacity_ = other.capacity_;
            data_ = std::move(other.data_);
            other.length = 0;
            other.capacity_ = 0;
            return *this;
        }

        // ---- Java-exact element access ----
        // Java indexes are signed int; negative must throw.
        reference operator[](jint i) {
            if (i < 0 || static_cast<size_type>(i) >= length) {
                throw_aioobe_(i, length);
            }
            return data_[static_cast<size_type>(i)];
        }

        const_reference operator[](jint i) const {
            if (i < 0 || static_cast<size_type>(i) >= length) {
                throw_aioobe_(i, length);
            }
            return data_[static_cast<size_type>(i)];
        }

        // Optional unsigned overloads (still Java-exact bounds check)
        reference operator[](size_type i) {
            if (i >= length) {
                throw_aioobe_(static_cast<jint>(i), length);
            }
            return data_[i];
        }

        const_reference operator[](size_type i) const {
            if (i >= length) {
                throw_aioobe_(static_cast<jint>(i), length);
            }
            return data_[i];
        }

        // Keep at() as alias; Java arrays only have checked []
        reference at(size_type i) { return (*this)[i]; }
        const_reference at(size_type i) const { return (*this)[i]; }

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
        void fill(const T& v) { std::fill(begin(), end(), v); }

        // ---- vector-like growth (kept) ----
        void reserve(size_type n) {
            if (n <= capacity_) return;
            grow_to_(n);
        }

        void push_back(const T& value) {
            ensure_capacity_for_push_();
            data_[length++] = value;
        }

        void push_back(T&& value) {
            ensure_capacity_for_push_();
            data_[length++] = std::move(value);
        }

        template <class... Args>
        reference emplace_back(Args&&... args) {
            ensure_capacity_for_push_();
            new (data_.get() + length) T(std::forward<Args>(args)...);
            return data_[length++];
        }

        friend bool operator==(const JxxArray& a, const JxxArray& b) {
            if (a.length != b.length) return false;
            for (size_type i = 0; i < a.length; ++i) {
                if (!(a[i] == b[i])) return false;
            }
            return true;
        }

        friend bool operator!=(const JxxArray& a, const JxxArray& b) { return !(a == b); }

        void swap(JxxArray& other) noexcept {
            using std::swap;
            swap(length, other.length);
            swap(capacity_, other.capacity_);
            swap(data_, other.data_);
        }

        void resize(size_type n) { resize(n, T{}); }

        void resize(size_type n, const T& fill_value) {
            if (n == length) return;

            if (n <= capacity_) {
                if (n > length) {
                    for (size_type i = length; i < n; ++i) data_[i] = fill_value;
                }
                length = n;
                return;
            }

            size_type new_cap = n;
            std::unique_ptr<T[]> buf(new T[new_cap]);
            std::copy_n(data_.get(), length, buf.get());
            for (size_type i = length; i < n; ++i) buf[i] = fill_value;
            data_.swap(buf);
            capacity_ = new_cap;
            length = n;
        }

    private:
        std::unique_ptr<T[]> data_{};
        size_type capacity_ = 0;

        static void throw_aioobe_(jint idx, size_type len) {
            (void)idx;
            (void)len;
            // Message content doesn't affect semantics; customize if you want.
            throw std::out_of_range("Index " + std::to_string(index) +
                " is out of range. Valid range: 0 to " +
                std::to_string(data.size() - 1));
        }

        void grow_to_(size_type need) {
            size_type new_cap = capacity_ ? capacity_ : size_type{ 4 };
            while (new_cap < need) {
                new_cap = (new_cap < (std::numeric_limits<size_type>::max() / 2))
                    ? (new_cap * 2)
                    : need;
            }
            std::unique_ptr<T[]> buf(new T[new_cap]);
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
    // Java parity:
    //   - outer holds references to SubArray => rows can be null
    //   - operator[] bounds-checks and throws ArrayIndexOutOfBoundsException
    //   - arr[i][j] throws NullPointerException if row is null
    //   - supports initializer including null rows:
    //       int[][] a = { null, {1,2}, null }
    // ======================================================================
    template <typename T, std::uint32_t Rank>
    class JxxArray {
        static_assert(Rank >= 2, "Use JxxArray<T,1> for rank-1 arrays");

    public:
        using SubArray = JxxArray<T, Rank - 1>;
        using size_type = std::uint32_t;
        using InitList = typename JInitList<T, Rank>::type;

        // Java-like public field
        size_type length = 0;

    private:
        std::vector<jxx::Ptr<SubArray>> elems_{};

        static void throw_aioobe_(jint idx, size_type len) {            
            throw std::out_of_range("JxxArray Index " + std::to_string(idx) +
                " is out of range. Valid range: 0 to " + std::to_string(len));
        }

        static void throw_npe_row_() {
            throw std::invalid_argument("JxxArray row is null");
        }

        static std::array<size_type, Rank - 1>
            tail_dims_(const std::array<size_type, Rank>& dims) {
            std::array<size_type, Rank - 1> tail{};
            for (std::size_t i = 1; i < Rank; ++i) tail[i - 1] = dims[i];
            return tail;
        }

    public:
        // Proxy for arr[i]:
        //   - assign nullptr / Ptr<SubArray>
        //   - allow chaining arr[i][j] with NPE if row is null
        class RowProxy {
        public:
            explicit RowProxy(jxx::Ptr<SubArray>* slot) : slot_(slot) {}

            RowProxy& operator=(std::nullptr_t) { *slot_ = nullptr; return *this; }
            RowProxy& operator=(const jxx::Ptr<SubArray>& p) { *slot_ = p; return *this; }
            RowProxy& operator=(jxx::Ptr<SubArray>&& p) { *slot_ = std::move(p); return *this; }

            explicit operator bool() const noexcept { return static_cast<bool>(*slot_); }

            // chaining arr[i][j]
            decltype(auto) operator[](jint j) {
                if (!*slot_) throw_npe_row_();
                return (**slot_)[j];
            }

            decltype(auto) operator[](jint j) const {
                if (!*slot_) throw_npe_row_();
                return (**slot_)[j];
            }

            // pointer access if needed
            jxx::Ptr<SubArray>& ptr() { return *slot_; }
            const jxx::Ptr<SubArray>& ptr() const { return *slot_; }

            SubArray* operator->() {
                if (!*slot_) throw_npe_row_();
                return slot_->get();
            }

            const SubArray* operator->() const {
                if (!*slot_) throw_npe_row_();
                return slot_->get();
            }

        private:
            jxx::Ptr<SubArray>* slot_;
        };

        class ConstRowProxy {
        public:
            explicit ConstRowProxy(const jxx::Ptr<SubArray>* slot) : slot_(slot) {}

            explicit operator bool() const noexcept { return static_cast<bool>(*slot_); }

            decltype(auto) operator[](jint j) const {
                if (!*slot_) throw_npe_row_();
                return (**slot_)[j];
            }

            const jxx::Ptr<SubArray>& ptr() const { return *slot_; }

            const SubArray* operator->() const {
                if (!*slot_) throw_npe_row_();
                return slot_->get();
            }

        private:
            const jxx::Ptr<SubArray>* slot_;
        };

        // ---- ctors ----

        JxxArray() = default;

        // Java: new T[n][] => n slots, rows null
        explicit JxxArray(size_type n) : length(n), elems_(n) {}

        // Java: new T[d0][d1]...[dN] => rectangular allocation (rows non-null)
        explicit JxxArray(const std::array<size_type, Rank>& dims) : length(dims[0]) {
            elems_.resize(length);
            auto tail = tail_dims_(dims);
            for (size_type i = 0; i < length; ++i) {
                elems_[i] = std::make_shared<SubArray>(tail);
            }
        }

        // Nested initializer (no null rows): { {1,2}, {3} }
        JxxArray(InitList init) : length(static_cast<size_type>(init.size())) {
            elems_.reserve(length);
            for (const auto& sub : init) {
                elems_.push_back(std::make_shared<SubArray>(sub));
            }
        }

        // ✅ Null-row initializer:
        //    int[][] a = { null, {1,2}, null }
        explicit JxxArray(std::initializer_list<jxx::Ptr<SubArray>> rows)
            : length(static_cast<size_type>(rows.size())), elems_(rows) {}

        // Copy/move
        JxxArray(const JxxArray&) = default;
        JxxArray& operator=(const JxxArray&) = default;
        JxxArray(JxxArray&&) noexcept = default;
        JxxArray& operator=(JxxArray&&) noexcept = default;

        // ---- Java-exact outer access (checked) ----
        RowProxy operator[](jint i) {
            if (i < 0 || static_cast<size_type>(i) >= length) {
                throw_aioobe_(i, length);
            }
            return RowProxy(&elems_[static_cast<size_type>(i)]);
        }

        ConstRowProxy operator[](jint i) const {
            if (i < 0 || static_cast<size_type>(i) >= length) {
                throw_aioobe_(i, length);
            }
            return ConstRowProxy(&elems_[static_cast<size_type>(i)]);
        }

        // Optional unsigned overloads (still checked)
        RowProxy operator[](size_type i) {
            if (i >= length) throw_aioobe_(static_cast<jint>(i), length);
            return RowProxy(&elems_[i]);
        }

        ConstRowProxy operator[](size_type i) const {
            if (i >= length) throw_aioobe_(static_cast<jint>(i), length);
            return ConstRowProxy(&elems_[i]);
        }

        // at() alias (Java arrays only have checked [])
        RowProxy at(size_type i) { return (*this)[i]; }
        ConstRowProxy at(size_type i) const { return (*this)[i]; }

        // Iterators over row pointers
        auto begin() noexcept { return elems_.begin(); }
        auto end() noexcept { return elems_.end(); }
        auto begin() const noexcept { return elems_.begin(); }
        auto end() const noexcept { return elems_.end(); }
        auto cbegin() const noexcept { return elems_.cbegin(); }
        auto cend() const noexcept { return elems_.cend(); }

        bool empty() const noexcept { return length == 0; }
        size_type size() const noexcept { return length; }
        size_type capacity() const noexcept { return static_cast<size_type>(elems_.capacity()); }

        void reserve(size_type n) { elems_.reserve(n); }

        // push_back row pointer (can be nullptr, Java-like)
        void push_back(const jxx::Ptr<SubArray>& row) {
            elems_.push_back(row);
            length = static_cast<size_type>(elems_.size());
        }

        void push_back(jxx::Ptr<SubArray>&& row) {
            elems_.push_back(std::move(row));
            length = static_cast<size_type>(elems_.size());
        }

        // Resize: new slots are null (Java default for reference arrays)
        void resize(size_type n) {
            elems_.resize(n);
            length = static_cast<size_type>(elems_.size());
        }

        // Fill only non-null rows
        void fill(const T& v) {
            for (auto& row : elems_) {
                if (row) row->fill(v);
            }
        }

        // Deep structural equality w/ null-row semantics
        friend bool operator==(const JxxArray& a, const JxxArray& b) {
            if (a.length != b.length) return false;
            for (size_type i = 0; i < a.length; ++i) {
                const auto& ra = a.elems_[i];
                const auto& rb = b.elems_[i];
                if (!ra || !rb) {
                    if (ra != rb) return false;
                }
                else {
                    if (!(*ra == *rb)) return false;
                }
            }
            return true;
        }

        friend bool operator!=(const JxxArray& a, const JxxArray& b) { return !(a == b); }

        void swap(JxxArray& other) noexcept {
            using std::swap;
            swap(length, other.length);
            elems_.swap(other.elems_);
        }
    };

} // namespace jxx::lang

// Convenience aliases (same as you had)
using ByteArray = jxx::lang::JxxArray<jxx::lang::jbyte, 1>;
using CharArray = jxx::lang::JxxArray<jxx::lang::jchar, 1>;
using IntArray = jxx::lang::JxxArray<jxx::lang::jint, 1>;
