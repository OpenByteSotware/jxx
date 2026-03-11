
#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <array>
#include <sstream>
#include "lang/jxx.lang.ByteType.h"

// Forward declare both
template <typename T, unsigned int N> class NDArray;
template <typename T> class ::NDArray<T, (int)1>;

// Specialization for N=1
template <typename T>
class ::NDArray<T, 1> {
    std::vector<T> dataVec;
public:
    const size_t length;
    NDArray(size_t size, const T& defaultValue = T())
        : dataVec(size, defaultValue), length(size) {}
    bool empty() const { return dataVec.empty(); }
    T* data() { return dataVec.data(); }
    const T* data() const { return dataVec.data(); }
    std::string toString() const {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < dataVec.size(); ++i) {
            oss << dataVec[i];
            if (i + 1 < dataVec.size()) oss << ", ";
        }
        oss << "]";
        return oss.str();
    }
};

// General case for N>1
template <typename T, unsigned int N>
class ::NDArray {
    std::vector< ::NDArray<T, N - 1> > dataVec; // MSVC-safe explicit global scope
public:
    const size_t length;
    template <typename... Sizes>
    NDArray(size_t firstDim, Sizes... otherDims)
        : length(firstDim) {
        static_assert(sizeof...(otherDims) == N - 1, "Dimension count mismatch");
        dataVec.reserve(firstDim);
        for (size_t i = 0; i < firstDim; ++i) {
            dataVec.emplace_back(otherDims...);
        }
    }
    bool empty() const { return dataVec.empty(); }
    T* data() { return dataVec.empty() ? nullptr : dataVec[0].data(); }
    const T* data() const { return dataVec.empty() ? nullptr : dataVec[0].data(); }
    std::string toString() const {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < dataVec.size(); ++i) {
            oss << dataVec[i].toString();
            if (i + 1 < dataVec.size()) oss << ", ";
        }
        oss << "]";
        return oss.str();
    }
};

//default 1D arrays that are much like java char[], byte[], etc
using CharArray = ::NDArray<char, (int)1>;
using ByteArray = ::NDArray<byte, (int)1>;
using IntArray = ::NDArray<int, (int)1>;
using FloatArray = ::NDArray<float, (int)1>;
