#pragma once

#include <cstddef>
#include <string>
#include <type_traits>
#include <utility>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Byte.h"
#include "lang/jxx.lang.Character.h"
#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.Float.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.Short.h"
#include "lang/jxx.lang.String.h"

namespace jxx {

    namespace generic_conversion_detail {

        template <typename Target, typename Source, typename = void>
        struct Converter;

        template <typename Target, typename Source, typename = void>
        struct IsConvertible : std::false_type {};

        template <typename Target, typename Source>
        struct IsConvertible<Target, Source, std::void_t<decltype(
            Converter<Target, std::decay_t<Source>>::convert(
                std::declval<Source>()))>> : std::true_type {};

        template <typename Target>
        struct Converter<Target, Target, void> {
            static Target convert(const Target& value) {
                return value;
            }

            static Target convert(Target&& value) {
                return std::move(value);
            }
        };

        template <typename T>
        struct Converter<::jxx::Ptr<T>, std::nullptr_t, void> {
            static ::jxx::Ptr<T> convert(std::nullptr_t) noexcept {
                return nullptr;
            }
        };

#define JXX_DEFINE_PRIMITIVE_BOXING(TargetClass, PrimitiveType) \
        template <> \
        struct Converter< \
            ::jxx::Ptr<::jxx::lang::TargetClass>, \
            ::jxx::lang::PrimitiveType, \
            void> { \
            static auto convert(::jxx::lang::PrimitiveType value) \
                -> ::jxx::Ptr<::jxx::lang::TargetClass> { \
                return ::jxx::lang::TargetClass::valueOf(value); \
            } \
        }

        JXX_DEFINE_PRIMITIVE_BOXING(Byte, jbyte);
        JXX_DEFINE_PRIMITIVE_BOXING(Short, jshort);
        JXX_DEFINE_PRIMITIVE_BOXING(Integer, jint);
        JXX_DEFINE_PRIMITIVE_BOXING(Long, jlong);
        JXX_DEFINE_PRIMITIVE_BOXING(Float, jfloat);
        JXX_DEFINE_PRIMITIVE_BOXING(Double, jdouble);
        JXX_DEFINE_PRIMITIVE_BOXING(Boolean, jbool);
        JXX_DEFINE_PRIMITIVE_BOXING(Character, jchar);

#undef JXX_DEFINE_PRIMITIVE_BOXING

        template <>
        struct Converter<
            ::jxx::Ptr<::jxx::lang::String>,
            const char*,
            void> {
            static auto convert(const char* value)
                -> ::jxx::Ptr<::jxx::lang::String> {
                return value == nullptr
                    ? nullptr
                    : ::jxx::NEW<::jxx::lang::String>(value);
            }
        };

        template <>
        struct Converter<
            ::jxx::Ptr<::jxx::lang::String>,
            char*,
            void> {
            static auto convert(const char* value)
                -> ::jxx::Ptr<::jxx::lang::String> {
                return value == nullptr
                    ? nullptr
                    : ::jxx::NEW<::jxx::lang::String>(value);
            }
        };

        template <>
        struct Converter<
            ::jxx::Ptr<::jxx::lang::String>,
            std::string,
            void> {
            static auto convert(const std::string& value)
                -> ::jxx::Ptr<::jxx::lang::String> {
                return ::jxx::NEW<::jxx::lang::String>(value);
            }

            static auto convert(std::string&& value)
                -> ::jxx::Ptr<::jxx::lang::String> {
                return ::jxx::NEW<::jxx::lang::String>(value);
            }
        };

    } // namespace generic_conversion_detail

    template <typename Target, typename Source>
    inline constexpr bool is_generic_convertible_v =
        generic_conversion_detail::IsConvertible<Target, Source>::value;

    template <typename Target, typename Source,
        std::enable_if_t<is_generic_convertible_v<Target, Source&&>, int> = 0>
    Target CONVERT(Source&& source) {
        return generic_conversion_detail::Converter<
            Target,
            std::decay_t<Source>>::convert(std::forward<Source>(source));
    }

    // A destination-typed element proxy for heterogeneous initializer lists.
    // It keeps primitive and string conversion rules centralized and does not
    // make jxx::Ptr implicitly constructible from arbitrary native values.
    template <typename Target>
    class GenericArgument final {
    public:
        GenericArgument(const Target& value)
            : value_(value) {}

        GenericArgument(Target&& value)
            : value_(std::move(value)) {}

        template <typename Source,
            std::enable_if_t<
                is_generic_convertible_v<Target, Source&&> &&
                !std::is_same_v<std::decay_t<Source>, Target>,
                int> = 0>
        GenericArgument(Source&& source)
            : value_(::jxx::CONVERT<Target>(std::forward<Source>(source))) {}

        const Target& get() const noexcept {
            return value_;
        }

    private:
        Target value_;
    };

} // namespace jxx
