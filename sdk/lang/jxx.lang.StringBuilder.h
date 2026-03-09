// StringBuilder.hpp
#pragma once

#include <string>
#include <string_view>
#include <charconv>     // for std::to_chars (integers are widely available in C++17)
#include <type_traits>
#include <limits>
#include <cstdio>       // for std::snprintf
#include <cassert>
#include "jxx.lang.Object.h"

namespace jxx::lang {

    class StringBuilder final {
    public:
        explicit StringBuilder(std::size_t capacity = 0) {
            if (capacity) buffer_.reserve(capacity);
        }

        // --- Core ---
        std::size_t size() const noexcept { return buffer_.size(); }
        std::size_t capacity() const noexcept { return buffer_.capacity(); }
        bool empty() const noexcept { return buffer_.empty(); }

        void reserve(std::size_t n) { buffer_.reserve(n); }
        void clear() noexcept { buffer_.clear(); }
        void shrink_to_fit() { buffer_.shrink_to_fit(); }

        // Java: setLength(len) -> truncate or pad with '\0' (NUL). We follow the same.
        void set_length(std::size_t len, char pad = '\0') {
            buffer_.resize(len, pad);
        }

        // Accessors similar to charAt / setCharAt
        char char_at(std::size_t idx) const {
            assert(idx < buffer_.size());
            return buffer_[idx];
        }
        void set_char_at(std::size_t idx, char c) {
            assert(idx < buffer_.size());
            buffer_[idx] = c;
        }

        // Subsequence (returns std::string_view)
        std::string_view subseq(std::size_t pos, std::size_t count) const {
            assert(pos <= buffer_.size());
            const auto n = (std::min)(count, buffer_.size() - pos);
            return std::string_view(buffer_).substr(pos, n);
        }

        // Replace (like Java's replace(start, end, str)) -> end is exclusive
        StringBuilder& replace(std::size_t start, std::size_t end, std::string_view s) {
            assert(start <= end && end <= buffer_.size());
            buffer_.replace(start, end - start, s.data(), s.size());
            return *this;
        }

        // Insert at position
        StringBuilder& insert(std::size_t pos, std::string_view s) {
            assert(pos <= buffer_.size());
            buffer_.insert(pos, s.data(), s.size());
            return *this;
        }

        // Delete range [start, end) (Java's delete)
        StringBuilder& erase(std::size_t start, std::size_t end) {
            assert(start <= end && end <= buffer_.size());
            buffer_.erase(start, end - start);
            return *this;
        }

        // Remove prefix/suffix by length (convenience)
        StringBuilder& remove_prefix(std::size_t n) {
            assert(n <= buffer_.size());
            buffer_.erase(0, n);
            return *this;
        }
        StringBuilder& remove_suffix(std::size_t n) {
            assert(n <= buffer_.size());
            buffer_.erase(buffer_.size() - n);
            return *this;
        }

        // toString() equivalent
        std::string str() const& { return buffer_; }
        std::string  str()&& { return std::move(buffer_); }

        // Direct view (non-owning)
        std::string_view view() const noexcept { return std::string_view{ buffer_ }; }

        // --- Append overloads (chainable) ---

        // Single char
        StringBuilder& append(char c) {
            buffer_.push_back(c);
            return *this;
        }

        // C-string (null-terminated)
        StringBuilder& append(const char* s) {
            if (s) buffer_.append(s);
            return *this;
        }

        // Raw bytes
        StringBuilder& append(const char* s, std::size_t len) {
            buffer_.append(s, len);
            return *this;
        }

        // std::string / string_view
        StringBuilder& append(std::string_view sv) {
            buffer_.append(sv.data(), sv.size());
            return *this;
        }
        StringBuilder& append(const std::string& s) {
            buffer_.append(s);
            return *this;
        }

        // Boolean (Java prints "true"/"false")
        StringBuilder& append(bool b) {
            return append(b ? "true" : "false");
        }

        // Integers – fast, locale-independent, allocation-free
        template <typename Int,
            typename = std::enable_if_t<std::is_integral<Int>::value && !std::is_same<Int, bool>::value>>
            StringBuilder & append(Int v, int base = 10) {
            // Max text length: sign + digits (base 2 worst-case) + null (not appended).
            // Safe upper bound using digits10 plus a little margin:
            char local[/*sign*/1 + std::numeric_limits<Int>::digits10 + 3];
            auto* first = local;
            auto* last = local + sizeof(local);
            auto res = std::to_chars(first, last, v, base);
            if (res.ec == std::errc()) {
                buffer_.append(first, static_cast<std::size_t>(res.ptr - first));
            }
            return *this;
        }

        // Floating-point – portable fallback using snprintf (you can adjust formatting)
        StringBuilder& append(double d, int precision = 6, bool scientific = false) {
            // Reserve enough: sign + digits + dot + exp + margin
            char local[64];
            const char* fmt = scientific ? "%.*e" : "%.*f";
            int n = std::snprintf(local, sizeof(local), fmt, precision, d);
            if (n > 0) {
                const auto count = static_cast<std::size_t>((n < static_cast<int>(sizeof(local))) ? n : (sizeof(local) - 1));
                buffer_.append(local, count);
            }
            return *this;
        }
        // float overload
        StringBuilder& append(float f, int precision = 6, bool scientific = false) {
            return append(static_cast<double>(f), precision, scientific);
        }
        // long double overload
        StringBuilder& append(long double ld, int precision = 6, bool scientific = false) {
            // snprintf with long double uses "%Lf"
            char local[80];
            const char* fmt = scientific ? "%.*Le" : "%.*Lf";
            int n = std::snprintf(local, sizeof(local), fmt, precision, ld);
            if (n > 0) {
                const auto count = static_cast<std::size_t>((n < static_cast<int>(sizeof(local))) ? n : (sizeof(local) - 1));
                buffer_.append(local, count);
            }
            return *this;
        }

        // Convenience: appendLine (like Java's append then newline)
        StringBuilder& append_line() {
            buffer_.push_back('\n');
            return *this;
        }
        template <class T>
        StringBuilder& append_line(const T& v) {
            return append(v).append('\n');
        }

        // Stream-like chaining via operator<< (uses append overloads)
        StringBuilder& operator<<(char c) { return append(c); }
        StringBuilder& operator<<(const char* s) { return append(s); }
        StringBuilder& operator<<(std::string_view sv) { return append(sv); }
        StringBuilder& operator<<(const std::string& s) { return append(s); }
        StringBuilder& operator<<(bool b) { return append(b); }
        StringBuilder& operator<<(short v) { return append(v); }
        StringBuilder& operator<<(unsigned short v) { return append(v); }
        StringBuilder& operator<<(int v) { return append(v); }
        StringBuilder& operator<<(unsigned int v) { return append(v); }
        StringBuilder& operator<<(long v) { return append(v); }
        StringBuilder& operator<<(unsigned long v) { return append(v); }
        StringBuilder& operator<<(long long v) { return append(v); }
        StringBuilder& operator<<(unsigned long long v) { return append(v); }
        StringBuilder& operator<<(float v) { return append(v); }
        StringBuilder& operator<<(double v) { return append(v); }
        StringBuilder& operator<<(long double v) { return append(v); }

    private:
        std::string buffer_;
    };

    template <class It>
    jxx::lang::String join(It first, It last, std::string_view sep) {
        StringBuilder sb;

        if (first == last)
        { 
            return jxx::lang::String{}; 
        }
        
        ++first;
        for (; first != last; ++first) {
            sb.append(sep);
            //sb.append(std::string_view{ *first });
        }
        
        return jxx::lang::String{ sb.str() };
    }


}