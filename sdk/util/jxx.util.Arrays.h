#pragma once

#include <algorithm>
#include <cstdint>
#include <sstream>
#include <string>
#include <type_traits>

#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.ClassCastException.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.NegativeArraySizeException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.Comparator.h"
#include "util/jxx.util.List.h"

namespace jxx::util {
namespace arrays_detail {

template <typename T>
struct IsPtr : std::false_type {};
template <typename T>
struct IsPtr<::jxx::Ptr<T>> : std::true_type {};

template <typename T>
inline bool equalValue(const T& left, const T& right) {
    if constexpr (IsPtr<T>::value) {
        if (left == right) return true;
        if (left == nullptr || right == nullptr) return false;
        const auto leftObject = ::jxx::CAST<::jxx::lang::Object>(left);
        const auto rightObject = ::jxx::CAST<::jxx::lang::Object>(right);
        return leftObject != nullptr && leftObject->equals(rightObject);
    } else {
        return left == right;
    }
}

template <typename T>
inline ::jxx::lang::jint hashValue(const T& value) {
    if constexpr (IsPtr<T>::value) {
        const auto object = ::jxx::CAST<::jxx::lang::Object>(value);
        return object == nullptr ? 0 : object->hashCode();
    } else if constexpr (std::is_same_v<T, ::jxx::lang::jbool>) {
        return value ? 1231 : 1237;
    } else if constexpr (std::is_floating_point_v<T>) {
        return static_cast<::jxx::lang::jint>(std::hash<T>{}(value));
    } else {
        return static_cast<::jxx::lang::jint>(value);
    }
}

template <typename T>
inline std::string textValue(const T& value) {
    if constexpr (IsPtr<T>::value) {
        const auto object = ::jxx::CAST<::jxx::lang::Object>(value);
        return object == nullptr ? "null" : object->toString()->utf8();
    } else if constexpr (std::is_same_v<T, ::jxx::lang::jbool>) {
        return value ? "true" : "false";
    } else if constexpr (std::is_same_v<T, ::jxx::lang::jchar>) {
        return ::jxx::NEW<::jxx::lang::String>(std::u16string(1, value))->utf8();
    } else {
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }
}

template <typename T>
inline bool lessValue(const T& left, const T& right) {
    if constexpr (IsPtr<T>::value) {
        if (left == nullptr || right == nullptr) throw ::jxx::lang::NullPointerException();
        const auto comparable = ::jxx::CAST<::jxx::lang::Comparable<typename T::element_type>>(left);
        if (comparable == nullptr) throw ::jxx::lang::ClassCastException();
        return comparable->compareTo(right) < 0;
    } else {
        return left < right;
    }
}

template <typename T>
inline void checkArray(const ::jxx::Ptr<::jxx::lang::JxxArray<T, 1U>>& array) {
    if (array == nullptr) throw ::jxx::lang::NullPointerException();
}

template <typename T>
inline void checkRange(const ::jxx::Ptr<::jxx::lang::JxxArray<T, 1U>>& array,
                       ::jxx::lang::jint from,
                       ::jxx::lang::jint to) {
    checkArray(array);
    if (from > to) throw ::jxx::lang::IllegalArgumentException();
    if (from < 0 || to > static_cast<::jxx::lang::jint>(array->length)) {
        throw ::jxx::lang::ArrayIndexOutOfBoundsException();
    }
}

} // namespace arrays_detail

class Arrays final {
public:
    Arrays() = delete;

    template <typename T>
    using Array = ::jxx::Ptr<::jxx::lang::JxxArray<T, 1U>>;

    template <typename T>
    static Array<T> copyOf(const Array<T>& original, ::jxx::lang::jint newLength) {
        arrays_detail::checkArray(original);
        if (newLength < 0) throw ::jxx::lang::NegativeArraySizeException();
        auto result = ::jxx::NEW<::jxx::lang::JxxArray<T, 1U>>(newLength);
        const auto count = std::min(newLength, static_cast<::jxx::lang::jint>(original->length));
        for (::jxx::lang::jint index = 0; index < count; ++index) (*result)[index] = (*original)[index];
        return result;
    }

    template <typename T>
    static Array<T> copyOfRange(const Array<T>& original,
                                ::jxx::lang::jint from,
                                ::jxx::lang::jint to) {
        arrays_detail::checkArray(original);
        if (from > to) throw ::jxx::lang::IllegalArgumentException();
        if (from < 0 || from > static_cast<::jxx::lang::jint>(original->length)) {
            throw ::jxx::lang::ArrayIndexOutOfBoundsException();
        }
        auto result = ::jxx::NEW<::jxx::lang::JxxArray<T, 1U>>(to - from);
        const auto count = std::min(to, static_cast<::jxx::lang::jint>(original->length)) - from;
        for (::jxx::lang::jint index = 0; index < count; ++index) (*result)[index] = (*original)[from + index];
        return result;
    }

    template <typename T>
    static void fill(const Array<T>& array, const T& value) {
        arrays_detail::checkArray(array);
        fill(array, 0, static_cast<::jxx::lang::jint>(array->length), value);
    }

    template <typename T>
    static void fill(const Array<T>& array, ::jxx::lang::jint from,
                     ::jxx::lang::jint to, const T& value) {
        arrays_detail::checkRange(array, from, to);
        for (auto index = from; index < to; ++index) (*array)[index] = value;
    }

    template <typename T>
    static ::jxx::lang::jbool equals(const Array<T>& left, const Array<T>& right) {
        if (left == right) return true;
        if (left == nullptr || right == nullptr || left->length != right->length) return false;
        for (::jxx::lang::jint index = 0; index < static_cast<::jxx::lang::jint>(left->length); ++index) {
            if (!arrays_detail::equalValue((*left)[index], (*right)[index])) return false;
        }
        return true;
    }

    template <typename T>
    static ::jxx::lang::jint hashCode(const Array<T>& array) {
        if (array == nullptr) return 0;
        ::jxx::lang::jint result = 1;
        for (::jxx::lang::jint index = 0; index < static_cast<::jxx::lang::jint>(array->length); ++index) {
            result = 31 * result + arrays_detail::hashValue((*array)[index]);
        }
        return result;
    }

    template <typename T>
    static ::jxx::Ptr<::jxx::lang::String> toString(const Array<T>& array) {
        if (array == nullptr) return ::jxx::NEW<::jxx::lang::String>("null");
        std::string result = "[";
        for (::jxx::lang::jint index = 0; index < static_cast<::jxx::lang::jint>(array->length); ++index) {
            if (index != 0) result += ", ";
            result += arrays_detail::textValue((*array)[index]);
        }
        result += "]";
        return ::jxx::NEW<::jxx::lang::String>(result);
    }

    template <typename T>
    static void sort(const Array<T>& array) {
        arrays_detail::checkArray(array);
        sort(array, 0, static_cast<::jxx::lang::jint>(array->length));
    }

    template <typename T>
    static void sort(const Array<T>& array, ::jxx::lang::jint from, ::jxx::lang::jint to) {
        arrays_detail::checkRange(array, from, to);
        std::sort(array->begin() + from, array->begin() + to,
                  [](const T& left, const T& right) { return arrays_detail::lessValue(left, right); });
    }

    template <typename T>
    static void sort(const Array<::jxx::Ptr<T>>& array,
                     const ::jxx::Ptr<Comparator<T>>& comparator) {
        arrays_detail::checkArray(array);
        if (comparator == nullptr) {
            sort(array);
            return;
        }
        std::sort(array->begin(), array->end(),
                  [&](const ::jxx::Ptr<T>& left, const ::jxx::Ptr<T>& right) {
                      return comparator->compare(left, right) < 0;
                  });
    }

    template <typename T>
    static ::jxx::lang::jint binarySearch(const Array<T>& array, const T& key) {
        arrays_detail::checkArray(array);
        ::jxx::lang::jint low = 0;
        ::jxx::lang::jint high = static_cast<::jxx::lang::jint>(array->length) - 1;
        while (low <= high) {
            const auto middle = low + ((high - low) >> 1);
            const auto& value = (*array)[middle];
            if (arrays_detail::lessValue(value, key)) low = middle + 1;
            else if (arrays_detail::lessValue(key, value)) high = middle - 1;
            else return middle;
        }
        return -low - 1;
    }

    template <typename T>
    static ::jxx::Ptr<List<T>> asList(const Array<::jxx::Ptr<T>>& array) {
        arrays_detail::checkArray(array);
        auto result = ::jxx::NEW<ArrayList<T>>(static_cast<::jxx::lang::jint>(array->length));
        for (::jxx::lang::jint index = 0; index < static_cast<::jxx::lang::jint>(array->length); ++index) {
            result->add((*array)[index]);
        }
        return ::jxx::CAST<List<T>>(result);
    }
};

} // namespace jxx::util
