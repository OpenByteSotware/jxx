#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Enumeration.h"

namespace jxx::util {

class StringTokenizer final
    : public ::jxx::lang::ClassBase<StringTokenizer, ::jxx::lang::Object>,
      public virtual Enumeration<::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<StringTokenizer, JxxSuper>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    StringTokenizer(
        const ::jxx::Ptr<::jxx::lang::String>& str,
        const ::jxx::Ptr<::jxx::lang::String>& delim,
        ::jxx::lang::jbool returnDelims);

    StringTokenizer(
        const ::jxx::Ptr<::jxx::lang::String>& str,
        const ::jxx::Ptr<::jxx::lang::String>& delim);

    template <typename Delimiter,
        std::enable_if_t<
            ::jxx::is_generic_convertible_v<
                ::jxx::Ptr<::jxx::lang::String>, Delimiter&&> &&
            !std::is_same_v<std::decay_t<Delimiter>,
                ::jxx::Ptr<::jxx::lang::String>>,
            int> = 0>
    StringTokenizer(
        const ::jxx::Ptr<::jxx::lang::String>& str,
        Delimiter&& delim,
        ::jxx::lang::jbool returnDelims)
        : StringTokenizer(
            str,
            ::jxx::CONVERT<::jxx::Ptr<::jxx::lang::String>>(
                std::forward<Delimiter>(delim)),
            returnDelims) {}

    template <typename Delimiter,
        std::enable_if_t<
            ::jxx::is_generic_convertible_v<
                ::jxx::Ptr<::jxx::lang::String>, Delimiter&&> &&
            !std::is_same_v<std::decay_t<Delimiter>,
                ::jxx::Ptr<::jxx::lang::String>>,
            int> = 0>
    StringTokenizer(
        const ::jxx::Ptr<::jxx::lang::String>& str,
        Delimiter&& delim)
        : StringTokenizer(
            str,
            ::jxx::CONVERT<::jxx::Ptr<::jxx::lang::String>>(
                std::forward<Delimiter>(delim))) {}

    explicit StringTokenizer(const ::jxx::Ptr<::jxx::lang::String>& str);

    ::jxx::lang::jbool hasMoreTokens();
    ::jxx::Ptr<::jxx::lang::String> nextToken();
    ::jxx::Ptr<::jxx::lang::String> nextToken(
        const ::jxx::Ptr<::jxx::lang::String>& delim);
    template <typename Delimiter,
        std::enable_if_t<
            ::jxx::is_generic_convertible_v<
                ::jxx::Ptr<::jxx::lang::String>, Delimiter&&> &&
            !std::is_same_v<std::decay_t<Delimiter>,
                ::jxx::Ptr<::jxx::lang::String>>,
            int> = 0>
    ::jxx::Ptr<::jxx::lang::String> nextToken(Delimiter&& delim) {
        return nextToken(
            ::jxx::CONVERT<::jxx::Ptr<::jxx::lang::String>>(
                std::forward<Delimiter>(delim)));
    }

    ::jxx::lang::jbool hasMoreElements() override;
    ::jxx::Ptr<::jxx::lang::Object> nextElement() override;
    ::jxx::lang::jint countTokens();

private:
    static ::jxx::lang::jint codePointAt_(
        const ::jxx::Ptr<::jxx::lang::String>& value,
        ::jxx::lang::jint index);
    static ::jxx::lang::jint charCount_(::jxx::lang::jint codePoint) noexcept;

    void requireDelimiters_() const;
    ::jxx::lang::jbool isDelimiter_(::jxx::lang::jint codePoint) const;
    ::jxx::lang::jint skipDelimiters_(::jxx::lang::jint startPosition) const;
    ::jxx::lang::jint scanToken_(::jxx::lang::jint startPosition) const;

    ::jxx::Ptr<::jxx::lang::String> str_;
    ::jxx::Ptr<::jxx::lang::String> delimiters_;
    ::jxx::lang::jbool returnDelimiters_ = false;
    ::jxx::lang::jint currentPosition_ = 0;
    ::jxx::lang::jint maxPosition_ = 0;
};

} // namespace jxx::util
