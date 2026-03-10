
#pragma once
#include <vector>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include "lang/jxx.lang.h"

namespace jxx { namespace util {

struct Arrays {
    template <typename T>
    static bool equals(const std::vector<T>& a, const std::vector<T>& b) {
        return a==b;
    }

    static bool equals(jxx::lang::ByteArray& a, jxx::lang::ByteArray& b) {
        if (a.size()!=b.size()) return false; return std::memcmp(a.data(), b.data(), a.size())==0;
    }

    static int hashCode(jxx::lang::ByteArray& a) {
        int h=1; for (size_t i=0;i<a.size();++i) h = 31*h + (int)a[i]; return h;
    }

    static jxx::lang::ByteArray copyOf(jxx::lang::ByteArray& a, int newLength) {
        if (newLength < 0) throw std::out_of_range("copyOf");
        jxx::lang::ByteArray out((size_t)newLength);
        size_t n = std::min<size_t>(a.size(), (size_t)newLength);
        std::memcpy(out.data(), a.data(), n);
        return out;
    }

    static void sort(jxx::lang::ByteArray& a) {
        std::sort(a.begin(), a.end());
    }
};

}} // namespace jxx::util
