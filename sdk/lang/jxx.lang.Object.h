#pragma once
#ifndef __JXX_OJBECT_H__
#define __JXX_OJBECT_H__

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

#include "lang/jxx.lang.Cloneable.h"
// ---------- Optional: demangle for GCC/Clang ----------
#if defined(__GNUG__) || defined(__clang__)
#include <cxxabi.h>
#include <cstdlib>
#endif

#include "jxx.lang.ByteType.h"
#include "jxx_types.h"

namespace jxx::lang {
    class Object;
    class String;
    class ClassAny;
}

namespace jxx::lang {

    inline std::string demangle(const char* name) {
#if defined(__GNUG__) || defined(__clang__)
        int status = 0;
        char* dem = abi::__cxa_demangle(name, nullptr, nullptr, &status);
        std::string out = (status == 0 && dem) ? dem : name;
        std::free(dem);
        return out;
#else
        (void)name;
        return "Object";
#endif
    }

 
    // =============== Object (root) ===============
    // Java-like root of the class hierarchy:
    // - Polymorphic for RTTI and dynamic_cast
    //  - Provides Java-like equals(), hashCode(), toString(), getClass(), and cloning semantics
    //  - Provides Java-like monitor methods: wait(), notify(), notifyAll() using condition_variable
    //  - Note: Java's Object is not thread-safe by default, but these methods allow you to use it as a monitor if desired. 
    //  - For Java-like synchronized blocks, use synchronized(obj, [&] { ... });
    //  - For Java-like synchronized methods, inherit from Synchronized below.
    //  - For Java-like polymorphic collections, use PolySet/PolyMap below.
    //  - For Java-like cloning, derive from Cloneable and implement cloneImpl() for deep copy. Object::clone() checks for Cloneable and delegates to cloneImpl().
    //  Note: we do NOT make Object copyable or assignable by default, since Java's Object is not. If you want copy semantics, derive from Cloneable and implement cloneImpl() for deep copy.
    //  - Because this pointer may be used as return need to inherit from enable_shared_from_this for safe shared_ptr creation in clone() and getClass()

    class Object : public std::enable_shared_from_this<Object> {
    public:
        // use thisPtr to return from methods if required to return this.  this cannot be retured as it is not a shared ptr
        // always use thisPtr when returning (if needed).  this->variable is ok to return, just cannot return this
        std::shared_ptr<Object> thisPtr;

        Object() = default;

        // make Object polymorphic for RTTI and dynamic_cast (destructor default)
        virtual ~Object();

        // Copy constructor: default-construct mutex and cv since they cannot be copied
        Object(const Object& other);

        // Copy assignment: similar to copy constructor
        Object& operator=(const Object& other);

        // Move constructor and assignment can be defaulted
        Object(Object&&) noexcept = default;
        Object& operator=(Object&&) noexcept = default;

        // Java-like: logical equality (default identity)
        virtual jbool equals(const jxx::Ptr<Object> other) const;

        // Java-like: hashCode (default identity-based)
        virtual jxx::lang::jint hashCode() const;

        jxx::Ptr<jxx::lang::ClassAny> getClass() const;

        // Class name (demangled where supported); override if you prefer custom names
        virtual jxx::Ptr<jxx::lang::String> getClassName() const;

        // Java-like: "Class@hexHash"
        virtual jxx::Ptr<jxx::lang::String> toString() const;

        // Identity check (reference equality)
        virtual bool same(const jxx::Ptr<Object> other) const;

        template <typename Rep, typename Period>
        bool wait_for(const std::chrono::duration<Rep, Period>& d) {
            std::unique_lock<std::mutex> lk(mtx_);
            return cv_.wait_for(lk, d) == std::cv_status::no_timeout;
        }

        void wait();

        void notify();

        void notifyAll();

        // Virtual clone method
        virtual jxx::Ptr<jxx::lang::Object> clone() const;

    protected:

        virtual jxx::Ptr<jxx::lang::Object> cloneImpl() const;

        template <typename T>
        std::shared_ptr<T> getThis() {
            static_assert(std::is_base_of<jxx::lang::Object, T>::value,
                "T must derive from Object");
            return std::dynamic_pointer_cast<T>(thisPtr);
        }

        // mimic Java 8 syncrhonized blocks: obj.synchronized([&] { ... });
        template <typename F>
        auto synchronized(F&& f) const -> decltype(f()) {
            std::lock_guard<std::recursive_mutex> guard(mutex_);
            return f();
        }

        mutable std::mutex mtx_;
        std::condition_variable cv_;

    protected:
        // release thisPtr as it was a reference to this object and we are being destroyed, so break the cycle
        void releaseSelf();

        jxx::Ptr<jxx::lang::String> getClassName_() const;

        mutable std::recursive_mutex mutex_;
    };

    class Cloneable {
    public:

        // make it polymorhpic
        virtual ~Cloneable() = default;

        // Implement cloneImpl for deep copy, Object uses this for C++ to mimic java like clone
        virtual jxx::Ptr<jxx::lang::Object> cloneImpl() const = 0;
    };


    // =============== Polymorphic hashing/equality for smart pointers ===============
    struct PolyHash {
        using is_transparent = void;
        std::size_t operator()(const std::shared_ptr<Object>& p) const {
            return p ? p->hashCode() : 0u;
        }
        std::size_t operator()(const Object* p) const {
            return p ? p->hashCode() : 0u;
        }
    };

    struct PolyEqual {
        using is_transparent = void;
        bool operator()(const std::shared_ptr<Object>& a,
            const std::shared_ptr<Object>& b) const {
            if (a == b) return true;
            if (!a || !b) return false;
            return a->equals(b);
        }
        bool operator()(const jxx::Ptr<Object> a,
            const jxx::Ptr<Object> b) const {
            if (!a || !b) return false;
            return a->equals(b);
        }
    };

    // Convenience aliases for polymorphic containers
    template <typename TPtr = jxx::Ptr<jxx::lang::Object>>
    using PolySet = std::unordered_set<TPtr, PolyHash, PolyEqual>;

    template <typename TValue, typename TKeyPtr = jxx::Ptr<jxx::lang::Object>>
    using PolyMap = std::unordered_map<TKeyPtr, TValue, PolyHash, PolyEqual>;



    //JXX_REGISTER_CLASS(jxx::lang::Object, "jxx.lang.Object", "Object");

    //JXX_REGISTER_INTERFACE(myns::IFoo, "myns.IFoo", "IFoo");
    //JXX_REGISTER_CLASS(myns::Foo, "myns.Foo", "Foo");

    //JXX_SET_SUPER(myns::Foo, jxx::lang::Object);
    //JXX_ADD_INTERFACE_EDGE(myns::Foo, myns::IFoo);

    //JXX_ENABLE_DEFAULT_CTOR(jxx::lang::Object);


#define JXX_OBJECT_CLONE(Derived) \
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override { return jxx::NEW<Derived>(*this); }
#endif



}

namespace jxx {
    // =====================================================
    // N-Dimensional Array Wrapper
    // =====================================================

    template <typename T, jxx::lang::jint N>
    class JxxArray {
    public:
        template <typename... Dims,
            typename = std::enable_if_t<sizeof...(Dims) == N &&
            std::conjunction_v<std::is_integral<Dims>...>>>
            explicit JxxArray(Dims... dims) : shape_{ static_cast<std::uint32_t>(dims)... } {
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
            std::array<std::size_t, N> indices{ static_cast<std::uint32_t>(idxs)... };
            return data_.get()[flat_index(indices)];
        }

        template <typename... Indices>
        const T& operator()(Indices... idxs) const {
            static_assert(sizeof...(Indices) == N, "Invalid number of indices");
            std::array<std::size_t, N> indices{ static_cast<std::uint32_t>(idxs)... };
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

        namespace detail {

            /*
             * Detects whether T exposes a writable thisPtr member.
             */
            template <typename T, typename = void>
            struct has_this_ptr
                : std::false_type {};

            template <typename T>
            struct has_this_ptr<
                T,
                std::void_t<
                decltype(
                    std::declval<T&>().thisPtr)>>
                : std::true_type {
            };

            /*
             * Applies JXX Object initialization after construction.
             */
            template <typename T>
            void initialize_object(
                const std::shared_ptr<T>& object) {

                if constexpr (has_this_ptr<T>::value) {
                    object->thisPtr = object;
                }
            }

        } // namespace detail

        /*
         * =====================================================================
         * Case 1: Fixed-size native C++ array type
         *
         * Usage:
         *
         *     auto value = jxx::NEW<int[10]>();
         *
         * Return type:
         *
         *     std::shared_ptr<std::array<int, 10>>
         *
         * This is native C++ convenience behavior. It is not JxxArray.
         * =====================================================================
         */
        template <
            typename T,
            std::enable_if_t<
            std::is_array_v<T> &&
            (std::extent_v<T> != 0U),
            int> = 0>
        auto NEW() {

            using ElementType =
                std::remove_extent_t<T>;

            constexpr std::size_t Count =
                std::extent_v<T>;

            return std::make_shared<
                std::array<
                ElementType,
                Count>>();
        }

        /*
         * =====================================================================
         * Case 2: Dynamic native C++ array type
         *
         * Usage:
         *
         *     auto value = jxx::NEW<int[]>(10);
         *
         * C++17 does not support:
         *
         *     std::make_shared<int[]>(10)
         *
         * so shared_ptr<T[]> is constructed directly.
         *
         * This is native C++ convenience behavior. For Java arrays, prefer
         * JxxArray<T, Rank>.
         * =====================================================================
         */
        template <
            typename T,
            std::enable_if_t<
            std::is_array_v<T> &&
            (std::extent_v<T> == 0U),
            int> = 0>
        auto NEW(
            std::size_t size) {

            using ElementType =
                std::remove_extent_t<T>;

            return std::shared_ptr<ElementType[]>(
                new ElementType,
                std::default_delete<ElementType[]>());
        }

        /*
         * =====================================================================
         * Case 3: Normal object construction
         *
         * This handles:
         *
         *     jxx::NEW<Vector<E>>(capacity, increment)
         *     jxx::NEW<String>("value")
         *     jxx::NEW<IntArrayType>(10)
         *     jxx::NEW<IntArray2DType>(2, 3)
         *     jxx::NEW<IntArray3DType>(2, 3, 4)
         *
         * JxxArray is a class, so its dimensional constructors work naturally
         * through this overload. No special multidimensional NEW overload is
         * required.
         * =====================================================================
         */
        template <
            typename T,
            typename... Args,
            std::enable_if_t<
            !std::is_array_v<T>,
            int> = 0>
        std::shared_ptr<T> NEW(
            Args&&... args) {            

            auto object =
                std::make_shared<T>(
                    std::forward<Args>(args)...);

            detail::initialize_object(object);

            return object;
        }

#ifndef CAST_PTR
#define CAST_PTR(Type, ptr) std::dynamic_pointer_cast<const Type>(ptr)
#endif
}

namespace jxx {

    // ---------------------------
    // Java-style instanceof
    // ---------------------------
    //
    // Usage:
    //   if (instanceof_as<Foo>(obj)) { ... }
    //
    // Semantics:
    //   - returns false if obj == null
    //   - returns true if obj is instance of Foo (or implements Foo if Foo is an interface type)
    //
    template <class To, class From>
    inline bool instanceof(const jxx::Ptr<From>& obj) noexcept {
        if (!obj) return false;
        return static_cast<bool>(std::dynamic_pointer_cast<To>(obj));
    }

    // ---------------------------
    // Java-style cast: (To) obj
    // ---------------------------
    //
    // Usage:
    //   auto foo = cast_as<Foo>(obj);     // returns null if obj is null; throws ClassCastException if incompatible
    //
    // Semantics:
    //   - if obj == null: returns null (same as Java)
    //   - else if compatible: returns casted pointer
    //   - else: throws ClassCastException
    //
    template <class To, class From>
    inline jxx::Ptr<To> cast_as(const jxx::Ptr<From>& obj) {
        if (!obj) return jxx::Ptr<To>{}; // Java: (T)null == null

        auto casted = std::dynamic_pointer_cast<To>(obj);
        if (casted) return casted;

        // Java: ClassCastException. Message text is not specified strictly; keep simple and stable.
        // If you want richer messages, you can use a registry or typeName() from Throwable/Object metadata.
        throw std::runtime_error("ClassCastException: incompatible cast");
    }

    // ---------------------------
    // Try-cast (safe cast): returns null instead of throwing
    // ---------------------------
    template <class To, class From>
    inline jxx::Ptr<To> try_cast_as(const jxx::Ptr<From>& obj) noexcept {
        if (!obj) return jxx::Ptr<To>{};
        return std::dynamic_pointer_cast<To>(obj);
    }

    template <typename To, typename From>
    jxx::Ptr<To> CAST(const std::shared_ptr<From>& ptr)
    {
        if constexpr (std::is_base_of_v<From, To>)
            return std::static_pointer_cast<To>(ptr);   // upcast (Derived -> Base)
        else
            return std::dynamic_pointer_cast<To>(ptr);  // downcast / unrelated
    } 

} // namespace jxx::lang
