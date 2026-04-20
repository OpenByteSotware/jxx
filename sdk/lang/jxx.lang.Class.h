#pragma once

#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include <functional>

#include "jxx.lang.String.h"
#include "jxx.lang.Object.h"

namespace jxx::lang {

struct TypeInfo;

// Non-template “Class<?>”
class ClassAny {
public:
    ClassAny() = default;
    explicit ClassAny(const TypeInfo* ti) : ti_(ti) {}

    jbool isNull() const { return ti_ == nullptr; }

    const String& getName() const;
    String getSimpleName() const;
    String getCanonicalName() const;

    // Java-ish relationships
    jbool isAssignableFrom(const ClassAny& other) const;
    jbool isInstance(const Object* obj) const;

    ClassAny getSuperclass() const;
    std::vector<ClassAny> getInterfaces() const;

    // Construction (Java8 Class#newInstance()-like). Returns Object*.
    // Throws/returns null depending on your exception model; here returns nullptr if not constructible.
    std::unique_ptr<Object> newInstance() const;

    const TypeInfo* raw() const { return ti_; }

private:
    const TypeInfo* ti_ = nullptr;
};

template <typename T>
class Class : public ClassAny {
public:
    using ClassAny::ClassAny;

    // Equivalent-ish to Java’s T.class (compile-time known)
    static Class<T> get();

    // Convenience typed checks
    jbool isInstance(const Object* obj) const {
        return ClassAny::isInstance(obj);
    }

    // Typed construction (if registered)
    std::unique_ptr<T> newInstanceTyped() const {
        auto base = ClassAny::newInstance();
        return std::unique_ptr<T>(dynamic_cast<T*>(base.release()));
    }
};

// ---- TypeInfo stored in registry ----

struct TypeInfo {
    String canonicalName;              // e.g. "java.util.Map" or "com.acme.Foo"
    String simpleName;                 // e.g. "Map" or "Foo"
    std::type_index type;              // RTTI identity

    const TypeInfo* super = nullptr;   // superclass metadata
    std::vector<const TypeInfo*> interfaces;

    // Construction function (optional)
    std::function<std::unique_ptr<Object>()> defaultCtor;

    // RTTI-based instance check (optional optimization)
    std::function<jbool(const Object*)> isInstanceFn;
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
        return (it == byName_.end()) ? nullptr : it->second;
    }

    const TypeInfo* findByType(std::type_index idx) const {
        std::lock_guard<std::mutex> lock(mu_);
        auto it = byType_.find(idx);
        return (it == byType_.end()) ? nullptr : it->second;
    }

private:
    TypeRegistry() = default;

    mutable std::mutex mu_;
    std::unordered_map<String, const TypeInfo*> byName_;
    std::unordered_map<std::type_index, const TypeInfo*> byType_;
};

// ---- ClassAny implementation ----

inline const String& ClassAny::getName() const {
    static const String empty = String("");
    return ti_ ? ti_->canonicalName : empty;
}

inline String ClassAny::getCanonicalName() const {
    return ti_ ? ti_->canonicalName : String("");
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

inline jbool ClassAny::isInstance(const Object* obj) const {
    if (!ti_ || !obj) return false;
    if (ti_->isInstanceFn) return ti_->isInstanceFn(obj);

    // Fallback: compare dynamic type registry match + inheritance chain check
    auto* dynTi = TypeRegistry::instance().findByType(std::type_index(typeid(*obj)));
    if (!dynTi) return false;
    return isAssignableFrom(ClassAny(dynTi));
}

inline jbool ClassAny::isAssignableFrom(const ClassAny& other) const {
    if (!ti_ || !other.ti_) return false;
    if (ti_->type == other.ti_->type) return true;

    // Walk superclass chain of other
    for (auto* cur = other.ti_->super; cur != nullptr; cur = cur->super) {
        if (cur->type == ti_->type) return true;
    }

    // Check interface list (direct + optionally inherited)
    // For full Java semantics, you’d want transitive interface closure; this is easy to extend.
    for (auto* iface : other.ti_->interfaces) {
        if (iface && iface->type == ti_->type) return true;
    }
    return false;
}

inline std::unique_ptr<Object> ClassAny::newInstance() const {
    if (!ti_ || !ti_->defaultCtor) return nullptr;
    return ti_->defaultCtor();
}

// ---- Class<T>::get() implementation ----

template <typename T>
inline Class<T> Class<T>::get() {
    static_assert(std::is_base_of_v<Object, T>,
                  "Class<T>::get requires T to derive from jxx::lang::Object (or adapt the constraint).");

    const TypeInfo* ti = TypeRegistry::instance().findByType(std::type_index(typeid(T)));
    return Class<T>(ti);
}

// ---- forName() ----
inline ClassAny forName(const String& canonicalName) {
    return ClassAny(TypeRegistry::instance().findByName(canonicalName));
}

} // namespace jxx::lang