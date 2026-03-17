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
#include <cstddef>
#include <stdexcept>
#include <cstdio>
#include <string>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <iostream>
#include "jxx_types.h"
#include "jxx.lang.Cloneable.h"

// ---------- Optional: demangle for GCC/Clang ----------
#if defined(__GNUG__) || defined(__clang__)
#include <cxxabi.h>
#include <cstdlib>

#endif

namespace jxx {
    namespace lang {

#define JXX_SYNCHRONIZE(obj, ...) obj->synchronized(__VA_ARGS__)
//#define SYNCHRONIZED_FUNC()

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
        
        class Object;

        class Cloneable {
        public:
            virtual ~Cloneable() = default;

            // Implement cloneImpl for deep copy, Ojbect uses this for C++ to mimic java like clone
            virtual std::shared_ptr<Object> cloneImpl() const = 0;
        };

        // =============== Object (root) ===============
        class Object {
        public:
            virtual ~Object() {};

            // Java-like: logical equality (default identity)
            virtual bool equals(const Object& other) const noexcept {
                return this == &other;
            }

            // Java-like: hashCode (default identity-based)
            virtual std::size_t hashCode() const noexcept {
                return std::hash<const void*>{}(this);
            }

            // Class name (demangled where supported); override if you prefer custom names
            virtual std::string getClassName() const {
#if defined(__GNUG__) || defined(__clang__) || defined(_MSC_VER)
                return demangle(typeid(*this).name());
#else
                return "Object";
#endif
            }

            // Java-like: "Class@hexHash"
            virtual std::string toString() const {
                std::ostringstream oss;
                oss << getClassName() << "@0x" << std::hex << hashCode();
                return oss.str();
            }

            // Identity check (reference equality)
            virtual bool same(const Object& other) const noexcept {
                return this == &other;
            }
            
            template <typename Rep, typename Period>
            bool wait_for(const std::chrono::duration<Rep, Period>& d) {
                std::unique_lock<std::mutex> lk(mtx_);
                return cv_.wait_for(lk, d) == std::cv_status::no_timeout;
            }
            void wait() {
                std::unique_lock<std::mutex> lk(mtx_);
                cv_.wait(lk);
            }

            void notify() {
                std::lock_guard<std::mutex> lg(mtx_);
                cv_.notify_one();
            }

            void notifyAll() {
                std::lock_guard<std::mutex> lg(mtx_);
                cv_.notify_all();
            }

            // Virtual clone method
            virtual std::shared_ptr<Object> clone() const {
                // Check if this object is Cloneable
                if (dynamic_cast<const Cloneable*>(this) == nullptr) {
                    throw std::runtime_error("CloneNotSupportedException");
                }
                // If Cloneable, delegate to derived class's cloneImpl
                return cloneImpl();
            }

        protected:

            virtual std::shared_ptr<Object> cloneImpl() const {
                throw std::runtime_error("cloneImpl not implemented");
            }

            mutable std::mutex mtx_;
            std::condition_variable cv_;
        };

       
        // =============== Synchronized mixin (Java-like monitor) ===============
        // Uses reentrant mutex (like Java's monitors).
        class Synchronized : public virtual Object {
        protected:
            mutable std::recursive_mutex mutex_;

        public:
            Synchronized() {};
            virtual ~Synchronized() {};

            Synchronized(const Synchronized&) {};

            // Execute functor under lock; returns functor's return value
            template <typename F>
            auto synchronized(F&& f) const -> decltype(f()) {
                std::lock_guard<std::recursive_mutex> guard(mutex_);
                return f();
            }

            // RAII guard if you need a multi-statement critical section
            class Guard {
                const Synchronized& obj_;
                std::lock_guard<std::recursive_mutex> lock_;
            public:
                explicit Guard(const Synchronized& o) : obj_(o), lock_(o.mutex_) {}
                // noncopyable
                Guard(const Guard&) = delete;
                Guard& operator=(const Guard&) = delete;
            };
        };

        // =============== Polymorphic hashing/equality for smart pointers ===============
        struct PolyHash {
            using is_transparent = void;
            std::size_t operator()(const std::shared_ptr<Object>& p) const noexcept {
                return p ? p->hashCode() : 0u;
            }
            std::size_t operator()(const Object* p) const noexcept {
                return p ? p->hashCode() : 0u;
            }
        };

        struct PolyEqual {
            using is_transparent = void;
            bool operator()(const std::shared_ptr<Object>& a,
                const std::shared_ptr<Object>& b) const noexcept {
                if (a == b) return true;
                if (!a || !b) return false;
                return a->equals(*b);
            }
            bool operator()(const std::shared_ptr<Object>& a,
                const Object* b) const noexcept {
                if (!a || !b) return false;
                return a->equals(*b);
            }
            bool operator()(const Object* a,
                const std::shared_ptr<Object>& b) const noexcept {
                if (!a || !b) return false;
                return b->equals(*a);
            }
        };

        // Convenience aliases for polymorphic containers
        template <typename TPtr = std::shared_ptr<Object>>
        using PolySet = std::unordered_set<TPtr, PolyHash, PolyEqual>;

        template <typename TValue, typename TKeyPtr = std::shared_ptr<Object>>
        using PolyMap = std::unordered_map<TKeyPtr, TValue, PolyHash, PolyEqual>;
} // namespace lang
} // namespace jxx

#endif
