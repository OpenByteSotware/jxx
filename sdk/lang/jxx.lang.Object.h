#pragma once
#ifndef __JXX_OBJECT_H__
#define __JXX_OBJECT_H__

#include <array>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#if defined(__GNUG__) || defined(__clang__)
#include <cxxabi.h>
#include <cstdlib>
#endif

#include "lang/jxx.lang.ByteType.h"
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx_types.h"

namespace jxx::lang
{
    class Object;
    class String;
    class ClassAny;
} // namespace jxx::lang

namespace jxx
{
    template <typename T, typename... Args>
    jxx::Ptr<T> NEW(Args&&... arguments);

    namespace detail
    {
        struct ObjectAccess;
    } // namespace detail
} // namespace jxx

namespace jxx::lang
{

    inline std::string demangle(const char* name)
    {
#if defined(__GNUG__) || defined(__clang__)
        int status = 0;
        char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
        std::string result = status == 0 && demangled != nullptr ? demangled : name;
        std::free(demangled);
        return result;
#else
        (void)name;
        return "Object";
#endif
    }

    class Object : public std::enable_shared_from_this<Object>
    {
    public:
        jxx::Ptr<Object> thisPtr() const;

        Object() = default;
        virtual ~Object();

        Object(const Object& other);
        Object& operator=(const Object& other);
        Object(Object&&) noexcept = default;
        Object& operator=(Object&&) noexcept = default;

        virtual jbool equals(const jxx::Ptr<Object>& other) const;
        virtual jxx::lang::jint hashCode() const;
        jxx::Ptr<jxx::lang::ClassAny> getClass() const;
        virtual jxx::Ptr<jxx::lang::String> getClassName() const;
        virtual jxx::Ptr<jxx::lang::String> toString() const;
        virtual bool same(const jxx::Ptr<Object>& other) const;

        template <typename Rep, typename Period>
        bool wait_for(const std::chrono::duration<Rep, Period>& duration)
        {
            std::unique_lock<std::mutex> lock(mtx_);
            return cv_.wait_for(lock, duration) == std::cv_status::no_timeout;
        }

        void wait();
        void notify();
        void notifyAll();

        virtual jxx::Ptr<jxx::lang::Object> clone() const;

        template <typename F>
        auto synchronized(F&& function) const -> decltype(function())
        {
            std::lock_guard<std::recursive_mutex> guard(mutex_);
            return function();
        }

    protected:
        virtual jxx::Ptr<jxx::lang::Object> cloneImpl() const;

        template <typename T>
        jxx::Ptr<T> getThis()
        {
            static_assert(
                std::is_base_of_v<jxx::lang::Object, T>,
                "T must derive from Object");
            return std::dynamic_pointer_cast<T>(thisPtr());
        }

        template <typename T>
        jxx::Ptr<const T> getThis() const
        {
            static_assert(
                std::is_base_of_v<jxx::lang::Object, T>,
                "T must derive from Object");
            return std::dynamic_pointer_cast<const T>(thisPtr());
        }

        mutable std::mutex mtx_;
        std::condition_variable cv_;

        void releaseSelf();
        jxx::Ptr<jxx::lang::String> getClassName_() const;
        mutable std::recursive_mutex mutex_;

    private:
        friend struct ::jxx::detail::ObjectAccess;

        void initializeThisPtr_(const jxx::Ptr<Object>& object) noexcept
        {
            thisPtr_ = object;
        }

        std::weak_ptr<Object> thisPtr_;
    };

    struct PolyHash
    {
        using is_transparent = void;

        std::size_t operator()(const std::shared_ptr<Object>& pointer) const
        {
            return pointer != nullptr ? pointer->hashCode() : 0u;
        }

        std::size_t operator()(const Object* pointer) const
        {
            return pointer != nullptr ? pointer->hashCode() : 0u;
        }
    };

    struct PolyEqual
    {
        using is_transparent = void;

        bool operator()(
            const std::shared_ptr<Object>& left,
            const std::shared_ptr<Object>& right) const
        {
            if (left == right) return true;
            if (left == nullptr || right == nullptr) return false;
            return left->equals(right);
        }
    };

    template <typename TPtr = jxx::Ptr<jxx::lang::Object>>
    using PolySet = std::unordered_set<TPtr, PolyHash, PolyEqual>;

    template <
        typename TValue,
        typename TKeyPtr = jxx::Ptr<jxx::lang::Object>>
        using PolyMap = std::unordered_map<TKeyPtr, TValue, PolyHash, PolyEqual>;

#define JXX_OBJECT_CLONE(Derived)                                      \
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override {           \
        return jxx::CAST<jxx::lang::Object>(jxx::NEW<Derived>(*this)); \
    }

} // namespace jxx::lang

namespace jxx
{

    template <typename T, jxx::lang::jint N>
    class JxxArray
    {
    public:
        template <
            typename... Dims,
            typename = std::enable_if_t<
            sizeof...(Dims) == N &&
            std::conjunction_v<std::is_integral<Dims>...>>>
            explicit JxxArray(Dims... dimensions)
            : shape_{ static_cast<std::uint32_t>(dimensions)... }
        {
            total_size_ = 1;
            for (const auto dimension : shape_) {
                if (dimension == 0) {
                    throw std::invalid_argument("Dimension size must be > 0");
                }
                total_size_ *= dimension;
            }
            data_ = std::shared_ptr<T>(
                new T[total_size_](),
                std::default_delete<T[]>());
        }

        template <typename... Indices>
        T& operator()(Indices... indices)
        {
            static_assert(sizeof...(Indices) == N, "Invalid number of indices");
            return data_.get()[flat_index(
                std::array<std::size_t, N>{
                static_cast<std::uint32_t>(indices)...})];
        }

        template <typename... Indices>
        const T& operator()(Indices... indices) const
        {
            static_assert(sizeof...(Indices) == N, "Invalid number of indices");
            return data_.get()[flat_index(
                std::array<std::size_t, N>{
                static_cast<std::uint32_t>(indices)...})];
        }

        const std::array<std::size_t, N>& shape() const
        {
            return shape_;
        }
        std::size_t size() const
        {
            return total_size_;
        }
        std::shared_ptr<T> data() const
        {
            return data_;
        }

    private:
        std::size_t flat_index(
            const std::array<std::size_t, N>& indices) const
        {
            std::size_t index = 0;
            std::size_t stride = 1;
            for (std::size_t dimension = N; dimension-- > 0;) {
                if (indices[dimension] >= shape_[dimension]) {
                    throw std::out_of_range("Index out of bounds");
                }
                index += indices[dimension] * stride;
                stride *= shape_[dimension];
            }
            return index;
        }

        std::array<std::size_t, N> shape_{};
        std::size_t total_size_ = 0;
        std::shared_ptr<T> data_;
    };

    namespace detail
    {
        struct ObjectAccess final
        {
            static void initialize(
                const jxx::Ptr<jxx::lang::Object>& object) noexcept
            {
                object->initializeThisPtr_(object);
            }
        };
    } // namespace detail

    template <typename T, typename... Args>
    jxx::Ptr<T> NEW(Args&&... arguments)
    {
        static_assert(
            !std::is_array_v<T>,
            "jxx::NEW<T> does not accept native C++ array types. "
            "Use JxxArray<T, Rank>.");

        static_assert(
            std::is_constructible_v<T, Args...>,
            "jxx::NEW<T>: T is not constructible from the supplied arguments.");

        auto object = std::make_shared<T>(
            std::forward<Args>(arguments)...);

        if constexpr (std::is_base_of_v<jxx::lang::Object, T>) {
            detail::ObjectAccess::initialize(
                std::static_pointer_cast<jxx::lang::Object>(object));
        }

        return object;
    }

#ifndef CAST_PTR
#define CAST_PTR(Type, ptr) std::dynamic_pointer_cast<const Type>(ptr)
#endif

    template <class To, class From>
    inline bool instanceof(const jxx::Ptr<From>& object) noexcept
    {
        if (object == nullptr) return false;
        return static_cast<bool>(std::dynamic_pointer_cast<To>(object));
    }

    template <class To, class From>
    inline jxx::Ptr<To> cast_as(const jxx::Ptr<From>& object)
    {
        if (object == nullptr) return nullptr;
        auto casted = std::dynamic_pointer_cast<To>(object);
        if (casted != nullptr) return casted;
        throw std::runtime_error("ClassCastException: incompatible cast");
    }

    template <class To, class From>
    inline jxx::Ptr<To> try_cast_as(const jxx::Ptr<From>& object) noexcept
    {
        if (object == nullptr) return nullptr;
        return std::dynamic_pointer_cast<To>(object);
    }

    template <typename To, typename From>
    jxx::Ptr<To> CAST(const jxx::Ptr<From>& pointer)
    {
        using ToType = std::remove_cv_t<To>;
        using FromType = std::remove_cv_t<From>;

        if (pointer == nullptr) return nullptr;

        if constexpr (std::is_same_v<ToType, FromType>) {
            return pointer;
        }
        else if constexpr (std::is_base_of_v<ToType, FromType>) {
            return std::static_pointer_cast<To>(pointer);
        }
        else {
            static_assert(
                std::is_polymorphic_v<FromType>,
                "jxx::CAST requires a polymorphic source type for checked casts.");
            return std::dynamic_pointer_cast<To>(pointer);
        }
    }

} // namespace jxx

#endif // __JXX_OBJECT_H__
