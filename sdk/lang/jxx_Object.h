#pragma once
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>

// ---------- Optional: demangle for GCC/Clang ----------
#if defined(__GNUG__) || defined(__clang__)
#include <cxxabi.h>
#include <cstdlib>
#endif

namespace jxx {
    namespace lang {

#define JXX_NEW(T, ...) std::make_shared<T>(__VA_ARGS__)
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
            bool same(const Object& other) const noexcept {
                return this == &other;
            }

            // Polymorphic clone (force derived to decide copy semantics)
            virtual std::shared_ptr<Object> clone() const = 0;
        };

        // =============== Cloneable mixin (CRTP) ===============
        template <typename Derived>
        class Cloneable : public virtual Object {
        public:
            virtual ~Cloneable() {};
            virtual std::shared_ptr<Object> clone() const override {
                return std::make_shared<Derived>(static_cast<const Derived&>(*this));
            }
        };

        // =============== Comparable mixin (like java.lang.Comparable) ===============
        template <typename Derived>
        class Comparable : public virtual Object {
        public:
            // Return negative/zero/positive like Java's compareTo
            virtual int compareTo(const Derived& other) const = 0;
            virtual ~Comparable() {};

            // Convenience operators
            bool operator<(const Derived& other)  const { return compareTo(other) < 0; }
            bool operator>(const Derived& other)  const { return compareTo(other) > 0; }
            bool operator<=(const Derived& other) const { return compareTo(other) <= 0; }
            bool operator>=(const Derived& other) const { return compareTo(other) >= 0; }
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

