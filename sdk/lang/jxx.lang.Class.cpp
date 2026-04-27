#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.ClassNotFoundException.h"
#include "lang/jxx.lang.IllegalAccessException.h"
#include "lang/jxx.lang.IllegalStateException.h"

using namespace jxx::lang;

    // forName is a template member - implementation moved to header

    jxx::lang::ClassAny::ClassAny(const jxx::lang::TypeInfo* ti) : ti_(ti) {}

    bool jxx::lang::ClassAny::isNull() const { return ti_ == nullptr; }
    const jxx::lang::TypeInfo* jxx::lang::ClassAny::raw() const { return this->ti_; }

    // Java-like identity (same "Class object" if same TypeInfo pointer)
    bool jxx::lang::ClassAny::operator==(const ClassAny& rhs) const noexcept { return ti_ == rhs.ti_; }
    bool jxx::lang::ClassAny::operator!=(const ClassAny& rhs) const noexcept { return ti_ != rhs.ti_; }
    // Java-like names
    const std::string& jxx::lang::ClassAny::getName() const {
        static const std::string empty{};
        return ti_ ? ti_->canonicalName : empty;
    }

    std::string jxx::lang::ClassAny::getSimpleName() const {
        return ti_ ? ti_->simpleName : std::string{};
    }

    // Java-like factory: Class.forName(...)
    jxx::lang::ClassAny jxx::lang::ClassAny::forName(const char* canonicalName) {
        const jxx::lang::TypeInfo* ti = TypeRegistry::instance().findByName(canonicalName);
        if (!ti) {
            // Replace with your String/exception constructors if needed
            throw jxx::lang::ClassNotFoundException("cannot find name for class");
        }
        return ClassAny(ti);
    }

    bool jxx::lang::ClassAny::isAssignableFrom(const ClassAny& other) const {
        if (!ti_ || !other.ti_) return false;

        // Exact type match
        if (ti_->type == other.ti_->type) return true;

        // Java parity: Object is assignable from ANY non-primitive reference type (including interfaces).
        // Use typeid(Object) instead of name to avoid name/init-order issues.
        const TypeInfo* objectTi = TypeRegistry::instance().findByType(std::type_index(typeid(Object)));
        if (objectTi && ti_->type == objectTi->type) {
            return !other.ti_->isPrimitive;
        }

        std::unordered_set<std::type_index> visited;
        return _reachable_type(other.ti_, ti_->type, visited);
    }

    bool jxx::lang::ClassAny::isInstance(const std::shared_ptr<Object> obj) const {
        if (!ti_ || !obj) return false;

        // Fast path if provided
        if (ti_->isInstanceFn) return ti_->isInstanceFn(obj);

        // Fallback: runtime type -> assignability
        const TypeInfo* dyn = TypeRegistry::instance().findByType(std::type_index(typeid(*obj)));
        if (!dyn) return false;
        return isAssignableFrom(ClassAny(dyn));
    }

    jxx::lang::ClassAny jxx::lang::ClassAny::getSuperclass() const {
        return jxx::lang::ClassAny(ti_ ? ti_->super : nullptr);
    }

    std::vector<jxx::lang::ClassAny> jxx::lang::ClassAny::getInterfaces() const {
        std::vector<jxx::lang::ClassAny> out;
        if (!ti_) return out;
        out.reserve(ti_->interfaces.size());
        for (auto* it : ti_->interfaces) out.emplace_back(it);
        return out;
    }

    // Java 8-like newInstance():
    // - InstantiationException if abstract/interface or no default ctor
    // - IllegalAccessException if default ctor exists but not accessible
    // - InvocationTargetException wraps exceptions thrown by ctor
    std::shared_ptr<jxx::lang::Object> jxx::lang::ClassAny::newInstance() {
        if (!ti_) {
            throw IllegalAccessException(/* "Cannot instantiate: null Class" */);
        }

        if (ti_->isInterface || ti_->isAbstract) {
            throw InstantiationException(/* "Cannot instantiate: " + ti_->canonicalName */);
        }

        if (!ti_->defaultCtor) {
            throw InstantiationException(/* "No default constructor for: " + ti_->canonicalName */);
        }

        if (!ti_->defaultCtorAccessible) {
            throw IllegalAccessException(/* "Default constructor not accessible for: " + ti_->canonicalName */);
        }

        try {
            return ti_->defaultCtor();
        }
        catch (const Exception& e) {
            throw InvocationTargetException(/* msg */, e);
        }
        catch (const std::exception& e) {
            throw InvocationTargetException(/* msg */, IllegalStateException(/* e.what() */));
        }
    }
