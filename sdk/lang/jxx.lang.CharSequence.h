#pragma once

#include "jxx_types.h"

namespace jxx::lang {

class String;

// Java interface: pure virtual, does NOT inherit from Object (per project rule)
struct CharSequence {
    virtual ~CharSequence() = default;

    virtual jint length() const = 0;
    virtual jchar charAt(jint index) const = 0;
    virtual jxx::Ptr<CharSequence> subSequence(jint beginIndex, jint endIndex) const = 0;

    // Java: toString() returns String
    virtual jxx::Ptr<String> toString() const = 0;
};

} // namespace jxx::lang
