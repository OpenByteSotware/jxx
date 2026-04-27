// Get all SDK, in the future can specify which ones to use
// but for now, it is all or nothing for ease of use
#pragma once

#include <cstdint>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <type_traits>
#include <numeric>
#include <array>
#include <stdexcept>
#include <cstdio>
#include <condition_variable>
#include <chrono>
#include <iostream>

// ---------- Optional: demangle for GCC/Clang ----------
#if defined(__GNUG__) || defined(__clang__)
#include <cxxabi.h>
#include <cstdlib>

#endif

namespace jxx::lang {
	// -------------------- Primitive types --------------------
	using jbyte = std::int8_t;     // Java byte:  8-bit signed
	using jshort = std::int16_t;    // Java short: 16-bit signed
	using jchar = char16_t;        // Java char:  16-bit UTF-16 code unit (unsigned in Java, but char16_t fits semantics)
	using jint = std::int32_t;    // Java int:   32-bit signed
	using jlong = std::int64_t;    // Java long:  64-bit signed
	using jfloat = float;           // Java float: IEEE-754 binary32 on typical targets
	using jdouble = double;          // Java double: IEEE-754 binary64 on typical targets
	using jbool = bool;            // Java boolean: logical true/false
}

// =====================================================
// N-Dimensional Array Wrapper
// =====================================================

template <typename T, std::size_t N>
class JxxArray {
public:
template <typename... Dims,
	typename = std::enable_if_t<sizeof...(Dims) == N &&
	std::conjunction_v<std::is_integral<Dims>...>>>
	explicit JxxArray(Dims... dims) : shape_{ static_cast<std::size_t>(dims)... } {
	total_size_ = 1;
	for (auto d : shape_) {
		if (d == 0) throw std::invalid_argument("Dimension size must be > 0");
		total_size_ *= d;
	}
	data_ = std::shared_ptr<T>(new T[total_size_](), std::default_delete<T[]>());
}

template <typename... Indices>
T& operator()(Indices... idxs) {
	static_assert(sizeof...(Indices) == N, "Invalid number of indices");
	std::array<std::size_t, N> indices{ static_cast<std::size_t>(idxs)... };
	return data_.get()[flat_index(indices)];
}

template <typename... Indices>
const T& operator()(Indices... idxs) const {
	static_assert(sizeof...(Indices) == N, "Invalid number of indices");
	std::array<std::size_t, N> indices{ static_cast<std::size_t>(idxs)... };
	return data_.get()[flat_index(indices)];
}

const std::array<std::size_t, N>& shape() const { return shape_; }
std::size_t size() const { return total_size_; }
std::shared_ptr<T> data() const { return data_; }

private:
std::size_t flat_index(const std::array<std::size_t, N>& indices) const {
	std::size_t idx = 0;
	std::size_t stride = 1;
	for (std::size_t dim = N; dim-- > 0;) {
		if (indices[dim] >= shape_[dim])
			throw std::out_of_range("Index out of bounds");
		idx += indices[dim] * stride;
		stride *= shape_[dim];
	}
	return idx;
}

std::array<std::size_t, N> shape_;
std::size_t total_size_;
std::shared_ptr<T> data_;
};

// =====================================================
// Auto-detect JXX_NEW
// =====================================================



// Case 1: Fixed-size arrays (T[N])
template <typename T,
	typename = std::enable_if_t<std::is_array_v<T>&& std::extent_v<T> != 0>>
	auto JXX_NEW() {
	using ElementType = std::remove_extent_t<T>;
	constexpr std::size_t N = std::extent_v<T>;
	return std::make_shared<std::array<ElementType, N>>();
}

// Case 2: Dynamic arrays (T[])
template <typename T,
	typename = std::enable_if_t<std::is_array_v<T>&& std::extent_v<T> == 0>>
	auto JXX_NEW(std::size_t size) {
	return std::make_shared<std::remove_extent_t<T>[]>(size);
}

// Case 3: Fully dynamic N-D arrays
// Enabled if: more than 1 integer arg OR (1 integer arg and T is not a class)
template <typename T, typename... Dims,
	typename = std::enable_if_t<!std::is_array_v<T> &&
	(sizeof...(Dims) > 1 ||
		(!std::is_class_v<T> && sizeof...(Dims) == 1)) &&
	std::conjunction_v<std::is_integral<Dims>...>>>
	auto JXX_NEW(Dims... dims) {
	constexpr std::size_t N = sizeof...(Dims);
	return JxxArray<T, N>(dims...);
}

// Case 4: Single object (default or with args)
// Enabled if: no args OR args not all integers OR T is a class
template <typename T, typename... Args,
	typename = std::enable_if_t<!std::is_array_v<T> &&
	(sizeof...(Args) == 0 ||
		!std::conjunction_v<std::is_integral<Args>...> ||
		std::is_class_v<T>)>>
	std::shared_ptr<T> JXX_NEW(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

#ifndef JXX_CAST_PTR
#define JXX_CAST_PTR(Type, ptr) std::dynamic_pointer_cast<const Type>(ptr)
#endif

namespace jxx {
	template <typename T>
	using Ptr = std::shared_ptr<T>;
}

#ifndef JXX_SYNCHRONIZE
#define JXX_SYNCHRONIZE(obj, ...) obj->synchronized(__VA_ARGS__)
#endif
