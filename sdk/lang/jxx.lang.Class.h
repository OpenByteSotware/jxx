#pragma once

#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <mutex>
#include <functional>
#include <string>
#include <stdexcept>

namespace jxx::lang {

    // ---- Forward declarations (you provide definitions elsewhere) ----
    class Object;

    // ---- Type metadata ----
    struct TypeInfo{
   
        std::string canonicalName;                     // e.g. "com.acme.Foo" / "java.lang.Object"
        std::string simpleName;                        // e.g. "Foo"
        std::type_index type;

        bool isInterface = false;
        bool isAbstract = false;
        bool isPrimitive = false;

        const TypeInfo* super = nullptr;               // superclass (null for interfaces)

        // For classes: implements edges
        // For interfaces: extends edges
        std::vector<const TypeInfo*> interfaces;

        // Default ctor factory (optional)
        bool defaultCtorAccessible = false;            // false => IllegalAccessException if factory exists
        std::function<std::shared_ptr<jxx::lang::Object>()> defaultCtor;

        // Instance check (optional, fast path)
        std::function<bool(const std::shared_ptr<jxx::lang::Object>&)> isInstanceFn;
    };

    // ---- Registry ----
    class TypeRegistry {
    public:
        static TypeRegistry& instance() {
            static TypeRegistry r;
            return r;
        }

        void registerType(const TypeInfo* ti) {
            std::lock_guard<std::mutex> lock(mu_);

            // Reject duplicates with different pointers (preserves "one Class object per type" semantics)
            auto itT = byType_.find(ti->type);
            if (itT != byType_.end() && itT->second != ti) {
                throw std::logic_error("TypeRegistry: duplicate registration for same C++ type");
            }

            auto itN = byName_.find(ti->canonicalName);
            if (itN != byName_.end() && itN->second != ti) {
                throw std::logic_error("TypeRegistry: duplicate registration for same canonical name");
            }

            byName_[ti->canonicalName] = ti;
            byType_[ti->type] = ti;
        }

        const TypeInfo* findByName(const char* name) const {
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
        std::unordered_map<std::string, const TypeInfo*> byName_;
        std::unordered_map<std::type_index, const TypeInfo*> byType_;
    };

    // ---- Helper: build a TypeInfo for T ----
    // NOTE: This assumes your object model uses std::shared_ptr<Object> and RTTI is enabled.
    template <typename T>
    inline TypeInfo makeTypeInfo(const char* canonical, const char* simple) {
        TypeInfo ti{
            std::string(canonical),
            std::string(simple),
            std::type_index(typeid(T)),
            false,   // isInterface
            false,   // isAbstract
            false,   // isPrimitive
            nullptr, // super
            {},      // interfaces
            false,   // defaultCtorAccessible
            nullptr, // defaultCtor
            // isInstanceFn: cross-cast works if implementing objects inherit both Object and interface T
            [](const std::shared_ptr<Object>& o) -> bool {
                return static_cast<bool>(std::dynamic_pointer_cast<T>(o));
            }
        };
        return ti;
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

    // =========================================================================
    // ClassAny: Java-like Class<?> handle
    // =========================================================================
    class ClassAny {
    public:
        ClassAny() = default;
        explicit ClassAny(const TypeInfo* ti);

        bool isNull() const;
        const TypeInfo* raw() const;
        
        // Java-like identity (same "Class object" if same TypeInfo pointer)
        bool operator==(const ClassAny& rhs) const noexcept;
        bool operator!=(const ClassAny& rhs) const noexcept;

        // Java-like names
        const std::string& getName() const;
        
        std::string getSimpleName() const;

        // Java-like factory: Class.forName(...)
        static ClassAny forName(const char* canonicalName);

        bool isAssignableFrom(const ClassAny& other) const;

        bool isInstance(const std::shared_ptr<jxx::lang::Object> obj) const;

        ClassAny getSuperclass() const;
        std::vector<ClassAny> getInterfaces() const;        
        std::shared_ptr<jxx::lang::Object> newInstance();
    protected:
        const TypeInfo* ti_ = nullptr;
    };

    // =========================================================================
    // Class<T>: typed wrapper
    // =========================================================================
    template <typename T>
    class Class : public ClassAny {
    public:
        using ClassAny::ClassAny;

        static Class<T> get() {
            static_assert(std::is_polymorphic_v<T>,
                "Class<T>::get requires T to be polymorphic (for RTTI/dynamic casts).");

            const TypeInfo* ti = TypeRegistry::instance().findByType(std::type_index(typeid(T)));
            return Class<T>(ti);
        }

        std::shared_ptr<T> newInstanceTyped() const {
            return std::dynamic_pointer_cast<T>(ClassAny::newInstance());
        }
    };

} // namespace jxx::lang