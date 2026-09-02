#pragma once

#include <type_traits>

namespace jxx::lang {

class Object;

template <
    typename Derived,
    typename JavaSuper,
    typename... JavaInterfaces>
class ClassInfo;

namespace class_info_detail {

template <typename T, typename = void>
struct HasClassInfo
    : std::false_type {
};

template <typename T>
struct HasClassInfo<
    T,
    std::void_t<typename T::JxxClassInfoMarker>>
    : std::true_type {
};

template <typename T>
inline constexpr bool HasClassInfoV =
    HasClassInfo<T>::value;

template <typename T>
void registerClassInfo();

} // namespace class_info_detail
} // namespace jxx::lang
