#pragma once
#include <type_traits>
//
// A C++17 pure abstract interface similar to Java 8's java.lang.Comparable<T>.
//
// Usage:
//   class MyType : public Comparable<MyType> {
//       int compareTo(const MyType& other) const override {
//           // return negative / zero / positive
//       }
//   };
//
// Contract (mirrors Java Comparable):
//   - Antisymmetry: sign(a.compareTo(b)) == -sign(b.compareTo(a))
//   - Transitivity: if a<b and b<c then a<c
//   - Consistency with equality (recommended):
//       a.compareTo(b) == 0  <=>  a == b (logical equality for your domain)
//   - Total order: For any a,b, compareTo yields consistent ordering
//
// Note: The operators below use compareTo for all comparisons, including ==/!=.
// If your equivalence notion differs from compareTo==0, do not rely on the
// provided ==/!=; define your own equality explicitly.
//

namespace jxx::lang {
    template <typename Derived>
    class Comparable {
        //virtual ~Comparable() = default;

        // Pure virtual: must be implemented by Derived
        virtual jint compareTo(const Derived& other) const = 0;
    };
}

    // ----- Relational operators derived from compareTo -----
    // These are enabled only for types Derived that inherit Comparable<Derived>.

    namespace detail {
        template <typename T>
        using is_valid_comparable_t =
            std::is_base_of<jxx::lang::Comparable<T>, T>;
    }

    // operator<
    template <typename T,
        typename = std::enable_if_t<detail::is_valid_comparable_t<T>::value>>
        constexpr bool operator<(const T& lhs, const T& rhs) {
        return lhs.compareTo(rhs) < 0;
    }

    // operator>
    template <typename T,
        typename = std::enable_if_t<detail::is_valid_comparable_t<T>::value>>
        constexpr bool operator>(const T& lhs, const T& rhs) {
        return lhs.compareTo(rhs) > 0;
    }

    // operator<=
    template <typename T,
        typename = std::enable_if_t<detail::is_valid_comparable_t<T>::value>>
        constexpr bool operator<=(const T& lhs, const T& rhs) {
        return lhs.compareTo(rhs) <= 0;
    }

    // operator>=
    template <typename T,
        typename = std::enable_if_t<detail::is_valid_comparable_t<T>::value>>
        constexpr bool operator>=(const T& lhs, const T& rhs) {
        return lhs.compareTo(rhs) >= 0;
    }

    // operator==
    template <typename T,
        typename = std::enable_if_t<detail::is_valid_comparable_t<T>::value>>
        constexpr bool operator==(const T& lhs, const T& rhs) {
        return lhs.compareTo(rhs) == 0;
    }

    // operator!=
    template <typename T,
        typename = std::enable_if_t<detail::is_valid_comparable_t<T>::value>>
        constexpr bool operator!=(const T& lhs, const T& rhs) {
        return lhs.compareTo(rhs) != 0;
    }
