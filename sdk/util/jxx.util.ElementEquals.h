#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::util {

template <class E>
struct ElementEquals {
    static bool eq(const E& a, const E& b) { return a == b; }
};

template <>
struct ElementEquals<jxx::Ptr<jxx::lang::Object>> {
    static bool eq(const jxx::Ptr<jxx::lang::Object>& a, const jxx::Ptr<jxx::lang::Object>& b) {
        if (!a && !b) return true;
        if (!a || !b) return false;
        return a->equals(b);
    }
};

} // namespace jxx::util
