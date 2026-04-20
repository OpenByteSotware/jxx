#pragma once

#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <mutex>
#include <functional>

#include "jxx.lang.String.h"
#include "jxx.lang.Object.h"

namespace jxx::lang {
    class Exception;
    class ClassNotFoundException;
    class InstantiationException;
    class IllegalAccessException;
    class InvocationTargetException;
} // namespace jxx::lang

namespace jxx::lang {

    struct TypeInfo;

    class ClassAny {
    public:
        ClassAny() = default;
        explicit ClassAny(const TypeInfo* ti) : ti_(ti) {}

        bool isNull() const { return ti_ == nullptr; }

        const String& getName() const;       // canonical, e.g. "com.acme.Foo"
        String getSimpleName() const;

        bool isAssignableFrom(const ClassAny& other) const;
        bool isInstance(const std::shared_ptr<Object>& obj) const;

        ClassAny getSuperclass() const;
        std::vector<ClassAny> getInterfaces() const;

        // Java 8-like: throws InstantiationException / IllegalAccessException / InvocationTargetException
        std::shared_ptr<Object> newInstance() const;

        const TypeInfo* raw() const { return ti_; }

    protected:
        const TypeInfo* ti_ = nullptr;
    };

    template <typename T>
    class Class : public ClassAny {
    public:
        using ClassAny::ClassAny;

        static Class<T> get() {
            // T may be an interface; do NOT require it derives Object.
            // But it must be polymorphic for RTTI and casts to work reliably.
            static_assert(std::has_virtual_destructor_v<T>,
                "Class<T>::get requires T to be polymorphic (virtual destructor).");

            const TypeInfo* ti = TypeRegistry::instance().findByType(std::type_index(typeid(T)));
            return Class<T>(ti);
        }

        std::shared_ptr<T> newInstanceTyped() const {
            // Returns nullptr if wrong type; newInstance() already throws on construction failures.
            return std::dynamic_pointer_cast<T>(ClassAny::newInstance());
        }
    };

    struct TypeInfo {
        String canonicalName;                     // "com.acme.Foo"
        String simpleName;                        // "Foo"
        std::type_index type;

        bool isInterface = false;
        bool isAbstract = false;                  // for Java parity
        bool isPrimitive = false;                 // optional

        const TypeInfo* super = nullptr;          // class superclass; for interfaces keep null

        // For classes: implements edges
        // For interfaces: extends edges
        std::vector<const TypeInfo*> interfaces;

        // Default ctor factory (optional)
        bool defaultCtorAccessible = false;       // if false and factory exists → IllegalAccessException
        std::function<std::shared_ptr<Object>()> defaultCtor;

        // Instance check: uses dynamic_pointer_cast to interface or class
        std::function<bool(const std::shared_ptr<Object>&)> isInstanceFn;
    };

    class TypeRegistry {
    public:
        static TypeRegistry& instance() {
            static TypeRegistry r;
            return r;
        }

        void registerType(const TypeInfo* ti) {
            std::lock_guard<std::mutex> lock(mu_);
            byName_[ti->canonicalName] = ti;
            byType_[ti->type] = ti;
        }

        const TypeInfo* findByName(const String& name) const {
            std::lock_guard<std::mutex> lock(mu_);
            auto it = byName_.find(name);
            return it == byName_.end() ? nullptr : it->second;
        }

        const TypeInfo* findByType(std::type_index idx) const {
            std::lock_guard<std::mutex> lock(mu_);
            auto it = byType_.find(idx);
            return it == byType_.end() ? nullptr : it->second;
        }

    private:
        TypeRegistry() = default;

        mutable std::mutex mu_;
        std::unordered_map<String, const TypeInfo*> byName_;
        std::unordered_map<std::type_index, const TypeInfo*> byType_;
    };

    // Java parity: throws ClassNotFoundException if not found
    inline ClassAny forName(const String& canonicalName) {
        const TypeInfo* ti = TypeRegistry::instance().findByName(canonicalName);
        if (!ti) {
            throw ClassNotFoundException(String("Class not found: ") + canonicalName);
        }
        return ClassAny(ti);
    }

    // ---- internal: full transitive closure through super + interfaces ----
    inline bool _reachable_type(const TypeInfo* start,
        const std::type_index& target,
        std::unordered_set<std::type_index>& visited) {
        if (!start) return false;
        if (start->type == target) return true;
        if (visited.find(start->type) != visited.end()) return false;
        visited.insert(start->type);

        // superclass chain
        if (_reachable_type(start->super, target, visited)) return true;

        // interface closure (covers both class implements and interface extends)
        for (auto* iface : start->interfaces) {
            if (_reachable_type(iface, target, visited)) return true;
        }
        return false;
    }

    // ---- ClassAny impl ----
    inline const String& ClassAny::getName() const {
        static const String empty = String("");
        return ti_ ? ti_->canonicalName : empty;
    }

    inline String ClassAny::getSimpleName() const {
        return ti_ ? ti_->simpleName : String("");
    }

    inline ClassAny ClassAny::getSuperclass() const {
        return ClassAny(ti_ ? ti_->super : nullptr);
    }

    inline std::vector<ClassAny> ClassAny::getInterfaces() const {
        std::vector<ClassAny> out;
        if (!ti_) return out;
        out.reserve(ti_->interfaces.size());
        for (auto* it : ti_->interfaces) out.emplace_back(it);
        return out;
    }

    inline bool ClassAny::isInstance(const std::shared_ptr<Object>& obj) const {
        if (!ti_ || !obj) return false;
        if (ti_->isInstanceFn) return ti_->isInstanceFn(obj);

        // fallback: runtime type -> assignability
        const TypeInfo* dyn = TypeRegistry::instance().findByType(std::type_index(typeid(*obj)));
        if (!dyn) return false;
        return isAssignableFrom(ClassAny(dyn));
    }

    inline bool ClassAny::isAssignableFrom(const ClassAny& other) const {
        if (!ti_ || !other.ti_) return false;

        // Exact type match
        if (ti_->type == other.ti_->type) return true;

        // Java parity: Object is assignable from ANY non-primitive reference type,
        // including interfaces.
        const TypeInfo* objectTi = TypeRegistry::instance().findByName(String("java.lang.Object"));
        if (objectTi && ti_->type == objectTi->type) {
            return !other.ti_->isPrimitive;
        }

        std::unordered_set<std::type_index> visited;
        return _reachable_type(other.ti_, ti_->type, visited);
    }

    inline std::shared_ptr<Object> ClassAny::newInstance() const {
        if (!ti_) {
            throw IllegalAccessException(String("Cannot instantiate: null Class"));
        }

        // Java 8 parity:
        // - interfaces and abstract classes cannot be instantiated
        if (ti_->isInterface || ti_->isAbstract) {
            throw InstantiationException(String("Cannot instantiate: ") + ti_->canonicalName);
        }

        // If no registered default ctor
        if (!ti_->defaultCtor) {
            throw InstantiationException(String("No default constructor for: ") + ti_->canonicalName);
        }

        // If ctor exists but not accessible (like private)
        if (!ti_->defaultCtorAccessible) {
            throw IllegalAccessException(String("Default constructor not accessible for: ") + ti_->canonicalName);
        }

        try {
            return ti_->defaultCtor();
        }
        catch (const Exception& e) {
            // Constructor threw a Java-like exception → wrap like reflection
            throw InvocationTargetException(String("Constructor threw for: ") + ti_->canonicalName, e);
        }
        catch (const std::exception& e) {
            // Wrap non-Java exception too
            throw InvocationTargetException(String("Constructor threw for: ") + ti_->canonicalName,
                IllegalStateException(String(e.what())));
        }
    }

} // namespace jxx::lang
