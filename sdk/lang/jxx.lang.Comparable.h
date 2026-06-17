#pragma once

#include <stdexcept>
#include "lang/jxx_types.h"

namespace jxx::lang {

    /**
     * Java 8: java.lang.Comparable<T>
     *
     * Contract:
     *   - compareTo(o) returns:
     *        < 0  if *this <  o
     *          0  if *this == o
     *        > 0  if *this >  o
     *   - compareTo(null) throws NullPointerException in Java
     *
     * jxx constraints:
     *   - Interfaces are pure virtual and DO NOT inherit from Object.
     *   - Java references are jxx::Ptr<T> (std::shared_ptr<T>).
     */
    template <class T>
    struct Comparable {
        virtual ~Comparable() = default;

        /**
         * Compare this object with another object.
         * Must throw on nullptr to match Java semantics.
         */
        virtual jxx::lang::jint compareTo(const jxx::Ptr<T> other) const = 0;

    protected:
        /**
         * Helper for Java parity: compareTo(nullptr) => NPE
         * Replace the throw type with your jxx::lang::NullPointerException when available.
         */
        static inline void throw_npe_if_null(const jxx::Ptr<T> p) {
            if (!p) {
                // If you already have jxx::lang::NullPointerException thrown by value, use that instead:
                // throw jxx::lang::NullPointerException(jxx::lang::String("..."));
                throw std::invalid_argument("NullPointerException: Comparable.compareTo(null)");
            }
        }
    };

} // namespace jxx::lang