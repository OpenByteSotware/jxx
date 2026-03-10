
#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <array>
#include <sstream>
#include <lang/jxx.lang.ByteArray.h>

template <typename T, size_t N> class NDArray;

// Recursive case: N > 1
template <typename T, size_t N>
class NDArray {
    std::vector<NDArray<T, N - 1>> dataVec;
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

    NDProxy<T, N> operator[](size_t index) {
        if (index >= dataVec.size()) throw std::out_of_range("Index out of range");
        return NDProxy<T, N>(dataVec);
    }
    const NDProxy<T, N> operator[](size_t index) const {
        if (index >= dataVec.size()) throw std::out_of_range("Index out of range");
        return NDProxy<T, N>(const_cast<std::vector<NDArray<T, N - 1>>&>(dataVec));
    }

    size_t size() const { return dataVec.size(); }

    std::array<size_t, N> dimensions() const {
        std::array<size_t, N> dims;
        dims[0] = dataVec.size();
        if (!dataVec.empty()) {
            auto subDims = dataVec[0].dimensions();
            for (size_t i = 0; i < N - 1; ++i) {
                dims[i + 1] = subDims[i];
            }
        }
        else {
            for (size_t i = 1; i < N; ++i) dims[i] = 0;
        }
        return dims;
    }

    // Return pointer to underlying contiguous block (deepest level)
    T* data() { return dataVec.empty() ? nullptr : dataVec[0].data(); }
    const T* data() const { return dataVec.empty() ? nullptr : dataVec[0].data(); }

    NDArray<T, N> clone() const {
        NDArray<T, N> copy = *this;
        return copy;
    }

    bool equals(const NDArray<T, N>& other) const {
        if (length != other.length) return false;
        for (size_t i = 0; i < length; ++i) {
            if (!dataVec[i].equals(other.dataVec[i])) return false;
        }
        return true;
    }

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

    void fill(const T& value) {
        for (auto& subArray : dataVec) {
            subArray.fill(value);
        }
    }

    NDArray<T, N> copyOfRange(size_t start, size_t end) const {
        if (start > end || end > data.size()) throw std::out_of_range("Invalid range");
        std::array<size_t, N> dims = dimensions();
        dims[0] = end - start;
        NDArray<T, N> result(dims[0], dims[1], (dims.size() > 2 ? dims[2] : 0)); // works for N=2 or N=3; can be generalized
        for (size_t i = start; i < end; ++i) {
            result.data[i - start] = data[i].clone();
        }
        return result;
    }
};

// Proxy for intermediate dimensions
template <typename T, size_t N>
class NDProxy {
    std::vector<NDArray<T, N - 1>>& subArrays;
public:
    NDProxy(std::vector<NDArray<T, N - 1>>& arr) : subArrays(arr) {}
    NDArray<T, N - 1>& operator[](size_t index) {
        if (index >= subArrays.size()) throw std::out_of_range("Index out of range");
        return subArrays[index];
    }
    const NDArray<T, N - 1>& operator[](size_t index) const {
        if (index >= subArrays.size()) throw std::out_of_range("Index out of range");
        return subArrays[index];
    }
};

// Base case: 1D array
template <typename T>
class NDArray<T, 1> {
    std::vector<T> dataVec;
public:
    const size_t length;

    NDArray(size_t size, const T& defaultValue = T())
        : dataVec(size, defaultValue), length(size) {}

    T& operator[](size_t index) {
        if (index >= dataVec.size()) throw std::out_of_range("Index out of range");
        return dataVec[index];
    }
    const T& operator[](size_t index) const {
        if (index >= dataVec.size()) throw std::out_of_range("Index out of range");
        return dataVec[index];
    }

    size_t size() const { return dataVec.size(); }

    std::array<size_t, 1> dimensions() const {
        return { dataVec.size() };
    }

    // Return pointer to underlying array
    T* data() { return dataVec.data(); }
    const T* data() const { return dataVec.data(); }

    NDArray<T, 1> clone() const {
        NDArray<T, 1> copy(length);
        copy.dataVec = dataVec;
        return copy;
    }

    bool equals(const NDArray<T, 1>& other) const {
        return dataVec == other.dataVec;
    }

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

    void fill(const T& value) {
        std::fill(dataVec.begin(), dataVec.end(), value);
    }
};



//default 1D arrays that are much like java char[], byte[], etc
using CharArray = NDArray<char, 1>;
using ByteArray = NDArray<byte, 1>;
using IntArray = NDArray<int, 1>;
using FloatArray = NDArray<float, 1>;
