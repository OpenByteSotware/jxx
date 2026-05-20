#pragma once

#include "jxx_types.h"

namespace jxx::lang {

/**
 * Java 8 parity: java.lang.CharSequence
 * Interface => does NOT inherit Object.
 */
class CharSequence {
public:
    virtual ~CharSequence() = default;

    virtual jxx::lang::jint length() const = 0;
    virtual jxx::lang::jchar charAt(jxx::lang::jint index) const = 0;
    virtual jxx::Ptr<CharSequence> subSequence(jxx::lang::jint start, jxx::lang::jint end) const = 0;
};

} // namespace jxx::lang
