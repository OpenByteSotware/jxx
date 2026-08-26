#pragma once

#include <string>
#include <string_view>

namespace type_name_util {

namespace detail {

template <typename T>
constexpr std::string_view
compiler_signature() noexcept {
#if defined(_MSC_VER)
    return __FUNCSIG__;
#elif defined(__clang__) || defined(__GNUC__)
    return __PRETTY_FUNCTION__;
#else
#   error Unsupported compiler
#endif
}

constexpr std::string_view
remove_type_keyword(
    std::string_view name) noexcept {

    constexpr std::string_view
        classPrefix = "class ";

    constexpr std::string_view
        structPrefix = "struct ";

    constexpr std::string_view
        enumPrefix = "enum ";

    if (name.substr(
            0,
            classPrefix.size()) ==
        classPrefix) {

        return name.substr(
            classPrefix.size());
    }

    if (name.substr(
            0,
            structPrefix.size()) ==
        structPrefix) {

        return name.substr(
            structPrefix.size());
    }

    if (name.substr(
            0,
            enumPrefix.size()) ==
        enumPrefix) {

        return name.substr(
            enumPrefix.size());
    }

    return name;
}

/**
 * Finds the first top-level '<'.
 *
 * A top-level template delimiter begins the template arguments of the
 * represented type itself.
 */
constexpr std::size_t
find_template_begin(
    std::string_view name) noexcept {

    for (std::size_t i = 0;
         i < name.size();
         ++i) {

        if (name[i] == '<') {
            return i;
        }
    }

    return std::string_view::npos;
}

/**
 * Finds the final namespace separator outside template arguments.
 */
constexpr std::size_t
find_last_outer_namespace_separator(
    std::string_view name) noexcept {

    std::size_t result =
        std::string_view::npos;

    std::size_t templateDepth = 0;

    for (std::size_t i = 0;
         i + 1 < name.size();
         ++i) {

        if (name[i] == '<') {
            ++templateDepth;
