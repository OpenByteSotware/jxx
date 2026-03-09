
#pragma once
#include <vector>
#include <memory>
#include "jxx.lang.h"

namespace jxx { namespace util {

struct Collections {
    template <typename T>
    static std::vector<T> emptyList() { return {}; }

    template <typename T>
    static std::vector<T> singletonList(const T& v) { return {v}; }
};

}} // namespace jxx::util
