#pragma once
#include <string>
#include "jxx_types.h"
#include "jxx.lang.String.h"

namespace jxx::lang {

// Java interface: pure virtual, does NOT inherit from Object (per project rule)
struct CharSequence {
    virtual ~CharSequence() = default;

    virtual jint length() const = 0;
    virtual jchar charAt(jint index) const = 0;
    virtual jxx::Ptr<CharSequence> subSequence(jint beginIndex, jint endIndex) const = 0;

    // Java: toString() returns String
    virtual jxx::Ptr<jxx::lang::String> toString() const = 0;
};

} // namespace jxx::lang
