
#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include <sstream>
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx { namespace util {

class Objects : public ::jxx::lang::ClassBase<Objects, ::jxx::lang::Object> {
    template <typename T> static bool equals(const T& a, const T& b) { return a==b; }
    template <typename T> static T requireNonNull(const T& obj) { /* if pointer-like, not handled here */ return obj; }
    template <typename T> static std::string toString(const T& obj) { std::ostringstream ss; ss<<obj; return ss.str(); }
};

}} // namespace jxx::util
