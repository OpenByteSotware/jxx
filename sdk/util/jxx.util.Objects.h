
#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include <sstream>
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx { namespace util {

class Objects : public ::jxx::lang::ClassBase<Objects, ::jxx::lang::Object> {
private:
    template <typename T>
    static bool nativeEquals_(const T& first, const T& second) {
        return first == second;
    }

    template <typename T> static T requireNonNull(const T& obj) { /* if pointer-like, not handled here */ return obj; }
    template <typename T> static std::string toString(const T& obj) { std::ostringstream ss; ss<<obj; return ss.str(); }

public:
    // Java 8 java.util.Objects.equals(Object, Object).
    static ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& first,
        const ::jxx::Ptr<::jxx::lang::Object>& second) {
        if (first == second) return true;
        if (first == nullptr || second == nullptr) return false;
        return first->equals(second);
    }

    // Typed JXX bridge for converted generic references such as Ptr<T>.
    template <typename First, typename Second>
    static ::jxx::lang::jbool equals(
        const ::jxx::Ptr<First>& first,
        const ::jxx::Ptr<Second>& second) {
        if (first == nullptr || second == nullptr) {
            return static_cast<::jxx::lang::jbool>(
                first == nullptr && second == nullptr);
        }
        return equals(
            ::jxx::CAST<::jxx::lang::Object>(first),
            ::jxx::CAST<::jxx::lang::Object>(second));
    }
};

}} // namespace jxx::util
