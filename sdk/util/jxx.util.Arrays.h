#pragma once

#include "lang/jxx.lang.ClassInfo.h"

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
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.JxxArray.h"
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
    if (from < 0 || to > static_cast<::jxx::lang::jint>(array->size())) {
        throw ::jxx::lang::ArrayIndexOutOfBoundsException();
    }
}


// Internal implementation detail for Arrays::asList().
// This is not a public source-parity type and must not appear in client APIs,
// class initialization, or serialization registration.
template <typename E>
class ArraysArrayBackedList final : public ::jxx::util::ArrayList<E> {
public:
    explicit ArraysArrayBackedList(
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<E>, 1U>>& array)
        : ::jxx::util::ArrayList<E>(0), array_(array) {}

    ::jxx::lang::jint size() override {
        return static_cast<::jxx::lang::jint>(array_->size());
    }
    ::jxx::lang::jbool isEmpty() override { return array_->size() == 0U; }
    ::jxx::Ptr<E> get(::jxx::lang::jint index) const override {
        checkIndex_(index); return (*array_)[index];
    }
    ::jxx::Ptr<E> set(::jxx::lang::jint index,
        const ::jxx::Ptr<E>& element) override {
        checkIndex_(index); auto old = (*array_)[index];
        (*array_)[index] = element; return old;
    }
    ::jxx::lang::jbool add(const ::jxx::Ptr<E>&) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    void add(::jxx::lang::jint, const ::jxx::Ptr<E>&) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    ::jxx::Ptr<E> remove(::jxx::lang::jint) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    ::jxx::lang::jbool remove(
        const ::jxx::Ptr<::jxx::lang::Object>&) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    void clear() override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    ::jxx::lang::jbool addAll(
        const ::jxx::Ptr<::jxx::util::wildcard::CollectionExtends<E>>&) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    ::jxx::lang::jbool addAll(::jxx::lang::jint,
        const ::jxx::Ptr<::jxx::util::wildcard::CollectionExtends<E>>&) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
private:
    void checkIndex_(::jxx::lang::jint index) const {
        if (index < 0 || index >= static_cast<::jxx::lang::jint>(array_->size()))
            throw ::jxx::lang::ArrayIndexOutOfBoundsException(index);
    }
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<E>, 1U>> array_;
};

} // namespace arrays_detail

class Arrays final : public ::jxx::lang::ClassBase<Arrays, ::jxx::lang::Object> {
public:
    Arrays() = delete;

    template <typename T>
    using Array = ::jxx::Ptr<::jxx::lang::JxxArray<T, 1U>>;

    template <typename T>
    static Array<T> copyOf(const Array<T>& original, ::jxx::lang::jint newLength) {
        arrays_detail::checkArray(original);
        if (newLength < 0) throw ::jxx::lang::NegativeArraySizeException();
        auto result = ::jxx::NEW<::jxx::lang::JxxArray<T, 1U>>(newLength);
        const auto count = std::min(newLength, static_cast<::jxx::lang::jint>(original->size()));
        for (::jxx::lang::jint index = 0; index < count; ++index)
        {
            (*result)[index] = (*original).at(index);
        }
        return result;
    }

    template <typename T>
    static Array<T> copyOfRange(const Array<T>& original,
                                ::jxx::lang::jint from,
                                ::jxx::lang::jint to) {
        arrays_detail::checkArray(original);
        if (from > to) throw ::jxx::lang::IllegalArgumentException();
        if (from < 0 || from > static_cast<::jxx::lang::jint>(original->size())) {
            throw ::jxx::lang::ArrayIndexOutOfBoundsException();
        }
        auto result = ::jxx::NEW<::jxx::lang::JxxArray<T, 1U>>(to - from);
        const auto count = std::min(to, static_cast<::jxx::lang::jint>(original->size())) - from;
        for (::jxx::lang::jint index = 0; index < count; ++index) (*result)[index] = (*original).at(from + index);
        return result;
    }

    template <typename T>
    static void fill(const Array<T>& array, const T& value) {
        arrays_detail::checkArray(array);
        fill(array, 0, static_cast<::jxx::lang::jint>(array->size()), value);
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
        if (left == nullptr || right == nullptr || left->size() != right->size()) return false;
        for (::jxx::lang::jint index = 0; index < static_cast<::jxx::lang::jint>(left->size()); ++index) {
            if (!arrays_detail::equalValue((*left).at(index), (*right).at(index))) return false;
        }
        return true;
    }

    template <typename T>
    static ::jxx::lang::jint hashCode(const Array<T>& array) {
        if (array == nullptr) return 0;
        ::jxx::lang::jint result = 1;
        for (::jxx::lang::jint index = 0; index < static_cast<::jxx::lang::jint>(array->size()); ++index) {
            result = 31 * result + arrays_detail::hashValue((*array)[index]);
        }
        return result;
    }

    template <typename T>
    static ::jxx::Ptr<::jxx::lang::String> toString(const Array<T>& array) {
        if (array == nullptr) return ::jxx::NEW<::jxx::lang::String>("null");
        std::string result = "[";
        for (::jxx::lang::jint index = 0; index < static_cast<::jxx::lang::jint>(array->size()); ++index) {
            if (index != 0) result += ", ";
            result += arrays_detail::textValue((*array).at(index));
        }
        result += "]";
        return ::jxx::NEW<::jxx::lang::String>(result);
    }

    template <typename T>
    static void sort(const Array<T>& array) {
        arrays_detail::checkArray(array);
        sort(array, 0, static_cast<::jxx::lang::jint>(array->size()));
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
        ::jxx::lang::jint high = static_cast<::jxx::lang::jint>(array->size()) - 1;
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
        auto result = ::jxx::NEW<arrays_detail::ArraysArrayBackedList<T>>(array);
        return ::jxx::CAST<List<T>>(result);
    }

    template <typename T, typename... Values,
        typename = std::enable_if_t<
            (std::is_convertible_v<Values, ::jxx::Ptr<T>> && ...)>>
    static ::jxx::Ptr<List<T>> asList(
        const ::jxx::Ptr<T>& first,
        const Values&... remaining) {
        auto array = ::jxx::NEW<::jxx::lang::JxxArray<
            ::jxx::Ptr<T>, 1U>>(
                static_cast<::jxx::lang::jint>(1U + sizeof...(remaining)));
        ::jxx::lang::jint index = 0;
        (*array)[index++] = first;
        (((*array)[index++] = static_cast<::jxx::Ptr<T>>(remaining)), ...);
        return asList<T>(array);
    }
};

} // namespace jxx::util
