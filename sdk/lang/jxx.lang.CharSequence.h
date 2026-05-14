#pragma once
#include <string>
#include "jxx_types.h"

namespace jxx::lang {

// Java interface: pure virtual, does NOT inherit from Object (per project rule)
struct CharSequence {
    virtual ~CharSequence() = default;

    virtual jint length() const = 0;
    virtual jchar charAt(jint index) const = 0;
    virtual jxx::Ptr<CharSequence> subSequence(jint beginIndex, jint endIndex) const = 0;

    // C++ doesn't allow same method signature.  This is a difference between c++ and java.
    // use Object toString instead
    // Java: toString() returns String
    //virtual jxx::Ptr<String> toString() const = 0;
};

} // namespace jxx::lang
