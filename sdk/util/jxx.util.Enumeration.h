#pragma once
#include "lang/jxx_types.h"

namespace jxx::util {

/**
 * Java 8 parity: java.util.Enumeration<E>
 * Interface => does NOT inherit Object.
 */
template <class E>
struct Enumeration {
    virtual ~Enumeration() = default;
    virtual jxx::lang::jbool hasMoreElements() = 0;
    virtual E nextElement() = 0;
};

} // namespace jxx::util