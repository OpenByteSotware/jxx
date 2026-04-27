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
#include "jxx.lang.Cast.h"

// ---------- Optional: demangle for GCC/Clang ----------
#if defined(__GNUG__) || defined(__clang__)
#include <cxxabi.h>
#include <cstdlib>

#endif

namespace jxx {
    namespace lang {

        class String;
		class ClassAny;


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

            // make it polymorhpic
            virtual ~Cloneable() = default;

            // Implement cloneImpl for deep copy, Ojbect uses this for C++ to mimic java like clone
            virtual jxx::Ptr<Object> cloneImpl() const = 0;
        };

        // =============== Object (root) ===============
		// Java-like root of the class hierarchy:
		// - Polymorphic for RTTI and dynamic_cast
		//  - Provides Java-like equals(), hashCode(), toString(), getClass(), and cloning semantics
		//  - Provides Java-like monitor methods: wait(), notify(), notifyAll() using condition_variable
		//  - Note: Java's Object is not thread-safe by default, but these methods allow you to use it as a monitor if desired. 
		//  - For Java-like synchronized blocks, use JXX_SYNCHRONIZE(obj, [&] { ... });
		//  - For Java-like synchronized methods, inherit from Synchronized below.
		//  - For Java-like polymorphic collections, use PolySet/PolyMap below.
		//  - For Java-like cloning, derive from Cloneable and implement cloneImpl() for deep copy. Object::clone() checks for Cloneable and delegates to cloneImpl().
		//  Note: we do NOT make Object copyable or assignable by default, since Java's Object is not. If you want copy semantics, derive from Cloneable and implement cloneImpl() for deep copy.
		//  - Because this pointer may be used as return need to inherit from enable_shared_from_this for safe shared_ptr creation in clone() and getClass()
        class Object : public std::enable_shared_from_this<Object> {
        public:

			// make Object polymorphic for RTTI and dynamic_cast (destructor default)
            virtual ~Object() = default;

            // Java-like: logical equality (default identity)
            virtual jbool equals(const jxx::Ptr<Object> other) const {
                return this == other.get();
            }

            // Java-like: hashCode (default identity-based)
            virtual jint hashCode() const {
                return std::hash<const void*>{}(this);
            }

            jxx::Ptr<ClassAny> getClass() const;


            // Class name (demangled where supported); override if you prefer custom names
            virtual jxx::Ptr<String> getClassName() const {
                return JXX_NEW<String>(this->getClassName_());
            }

            // Java-like: "Class@hexHash"
            virtual jxx::Ptr<String>  toString() const {
                std::ostringstream oss;
                oss << getClassName_() << "@0x" << std::hex << hashCode();
                return JXX_NEW<String>(oss.str());
            }

            // Identity check (reference equality)
            virtual bool same(const Object& other) const {
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
            virtual jxx::Ptr<Object> clone() const {
                // Check if this object is Cloneable
                if (JXX_CAST_PTR(Cloneable, shared_from_this()) == nullptr) {
                    throw std::runtime_error("CloneNotSupportedException");
                }
                // If Cloneable, delegate to derived class's cloneImpl
                return cloneImpl();
            }

        protected:

            virtual void finalize() {
                // Default implementation does nothing; override for cleanup if desired.
			}

            virtual jxx::Ptr<Object> cloneImpl() const {
                throw std::runtime_error("cloneImpl not implemented");
            }

            mutable std::mutex mtx_;
            std::condition_variable cv_;

        private:
            std::string getClassName_() const {
#if defined(__GNUG__) || defined(__clang__) || defined(_MSC_VER)
                return std::string(demangle(typeid(*this).name()));
#else
                return std::string("Object");
#endif
            }
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
            bool operator()(const jxx::Ptr<Object> a,
                const jxx::Ptr<Object> b) const {
                if (!a || !b) return false;
                return a->equals(b);
            }
        };

        // Convenience aliases for polymorphic containers
        template <typename TPtr = jxx::Ptr<Object>>
        using PolySet = std::unordered_set<TPtr, PolyHash, PolyEqual>;

        template <typename TValue, typename TKeyPtr = jxx::Ptr<Object>>
        using PolyMap = std::unordered_map<TKeyPtr, TValue, PolyHash, PolyEqual>;

} // namespace lang
} // namespace jxx

JXX_REGISTER_CLASS(jxx::lang::Object, "java.lang.Object", "Object");

#define JXX_OBJECT_CLONE(Derived) \
    JXX_PTR(Object) cloneImpl() const override { \
        return std::make_shared<Derived>(*this); \
    }

#endif
