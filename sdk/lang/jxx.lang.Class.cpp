#include "jxx.lang.String.h"
#include "jxx.lang.Class.h"

namespace jxx::lang {

    std::mutex ClassAny::registryMutex_{};
    std::unordered_map<std::string, std::weak_ptr<ClassAny>> ClassAny::registry_{};

    ClassAny::ClassAny(Meta meta) : meta_(std::move(meta)) {}

    jxx::Ptr<ClassAny> ClassAny::registerClass(const Meta& meta) {
        if (meta.binaryName.empty()) {
            throw IllegalArgumentException(JXX_NEW<String>("ClassAny.registerClass: binaryName is empty"));
        }

        std::lock_guard<std::mutex> lk(registryMutex_);

        // Return existing if present and alive
        auto it = registry_.find(meta.binaryName);
        if (it != registry_.end()) {
            if (auto existing = it->second.lock()) {
                return existing;
            }
        }

        auto cls = JXX_NEW<ClassAny>(meta);
        registry_[meta.binaryName] = cls;
        return cls;
    }

    jxx::Ptr<ClassAny> ClassAny::forName(jxx::Ptr<String> className) {
        if (!className) {
            throw NullPointerException(JXX_NEW<String>("className"));
        }
        auto key = className->utf8();

        std::lock_guard<std::mutex> lk(registryMutex_);
        auto it = registry_.find(key);
        if (it != registry_.end()) {
            if (auto c = it->second.lock()) return c;
        }
        throw ClassNotFoundException(className);
    }

    jxx::Ptr<String> ClassAny::getName() const {
        return JXX_NEW<String>(meta_.binaryName.c_str());
    }

    std::string ClassAny::simpleNameFromBinary_(const std::string& bin) {
        auto pos = bin.find_last_of('.');
        if (pos == std::string::npos) return bin;
        return bin.substr(pos + 1);
    }

    jxx::Ptr<String> ClassAny::getSimpleName() const {
        // Java has subtle rules for anonymous/local classes; we keep basic parity here.
        return JXX_NEW<String>(simpleNameFromBinary_(meta_.binaryName).c_str());
    }

    jxx::Ptr<String> ClassAny::getCanonicalName() const {
        // Java returns null for anonymous/local. If you need that, add flags to Meta.
        return getName();
    }

    jxx::Ptr<String> ClassAny::toString() const {
        std::string s = meta_.isInterface ? "interface " : "class ";
        s += meta_.binaryName;
        return JXX_NEW<String>(s.c_str());
    }

    jint ClassAny::getModifiers() const { return meta_.modifiers; }

    jbool ClassAny::isInterface() const { return meta_.isInterface; }
    jbool ClassAny::isPrimitive() const { return meta_.isPrimitive; }
    jbool ClassAny::isArray() const { return meta_.isArray; }
    jbool ClassAny::isEnum() const { return meta_.isEnum; }
    jbool ClassAny::isAnnotation() const { return meta_.isAnnotation; }
    jbool ClassAny::isSynthetic() const { return meta_.isSynthetic; }

    jxx::Ptr<ClassAny> ClassAny::getSuperclass() const { return meta_.superClass; }

    jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> ClassAny::toClassArray_(const std::vector<jxx::Ptr<ClassAny>>& v) {
        auto a = JXX_NEW<JxxArray<jxx::Ptr<ClassAny>, 1>>((std::uint32_t)v.size());
        for (std::uint32_t i = 0; i < (std::uint32_t)v.size(); ++i) {
            (*a)[i] = v[i];
        }
        return a;
    }

    jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> ClassAny::getInterfaces() const {
        return toClassArray_(meta_.interfaces);
    }

    jxx::Ptr<ClassAny> ClassAny::getComponentType() const {
        return meta_.componentType;
    }

    jbool ClassAny::isAssignableFromImpl_(const ClassAny& target, const ClassAny& src) {
        // target.isAssignableFrom(src) == true if src is same type or subclass or implements interface
        if (target.meta_.typeId == src.meta_.typeId) return true;

        // Walk superclass chain
        for (auto cur = src.meta_.superClass; cur; cur = cur->meta_.superClass) {
            if (cur->meta_.typeId == target.meta_.typeId) return true;
        }

        // Check interfaces recursively
        std::vector<jxx::Ptr<ClassAny>> stack(src.meta_.interfaces.begin(), src.meta_.interfaces.end());
        while (!stack.empty()) {
            auto top = stack.back();
            stack.pop_back();
            if (!top) continue;
            if (top->meta_.typeId == target.meta_.typeId) return true;
            for (auto& ii : top->meta_.interfaces) stack.push_back(ii);
        }

        return false;
    }

    jbool ClassAny::isAssignableFrom(jxx::Ptr<ClassAny> cls) const {
        if (!cls) throw NullPointerException(JXX_NEW<String>("cls"));
        return isAssignableFromImpl_(*this, *cls);
    }

    jbool ClassAny::isInstance(jxx::Ptr<Object> obj) const {
        if (!obj) return false;
        // Requires Object::getClass() in your runtime for exact parity. If you have it, use it.
        // Here we use RTTI fallback: compare type_index if possible.
        auto* o = obj.get();
        // If the object can supply class info, prefer that:
        // auto c = obj->getClass(); return isAssignableFrom(c);
        // Fallback: RTTI exact match only
        return (meta_.typeId == std::type_index(typeid(*o)));
    }

    jxx::Ptr<Object> ClassAny::cast(jxx::Ptr<Object> obj) const {
        if (!obj) return nullptr;
        // If you have Object::getClass(), use isAssignableFrom. Otherwise, best-effort:
        if (isInstance(obj)) return obj;
        throw ClassCastException(JXX_NEW<String>(meta_.binaryName.c_str()));
    }

    jxx::Ptr<Object> ClassAny::newInstance() const {
        if (!meta_.factory) {
            // Java throws InstantiationException for interfaces/abstract etc.
            throw InstantiationException(JXX_NEW<String>(meta_.binaryName.c_str()));
        }
        return meta_.factory();
    }

    // ---------------- Reflection stubs ----------------
    jxx::Ptr<JxxArray<jxx::Ptr<Field>, 1>> ClassAny::getFields() const {
        return JXX_NEW<JxxArray<jxx::Ptr<Field>, 1>>(0);
    }
    jxx::Ptr<JxxArray<jxx::Ptr<Field>, 1>> ClassAny::getDeclaredFields() const {
        return JXX_NEW<JxxArray<jxx::Ptr<Field>, 1>>(0);
    }
    jxx::Ptr<Field> ClassAny::getField(jxx::Ptr<String> /*name*/) const { return nullptr; }
    jxx::Ptr<Field> ClassAny::getDeclaredField(jxx::Ptr<String> /*name*/) const { return nullptr; }

    jxx::Ptr<JxxArray<jxx::Ptr<Method>, 1>> ClassAny::getMethods() const {
        return JXX_NEW<JxxArray<jxx::Ptr<Method>, 1>>(0);
    }
    jxx::Ptr<JxxArray<jxx::Ptr<Method>, 1>> ClassAny::getDeclaredMethods() const {
        return JXX_NEW<JxxArray<jxx::Ptr<Method>, 1>>(0);
    }
    jxx::Ptr<Method> ClassAny::getMethod(jxx::Ptr<String> /*name*/, jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> /*pt*/) const { return nullptr; }
    jxx::Ptr<Method> ClassAny::getDeclaredMethod(jxx::Ptr<String> /*name*/, jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> /*pt*/) const { return nullptr; }

    jxx::Ptr<JxxArray<jxx::Ptr<Constructor>, 1>> ClassAny::getConstructors() const {
        return JXX_NEW<JxxArray<jxx::Ptr<Constructor>, 1>>(0);
    }
    jxx::Ptr<JxxArray<jxx::Ptr<Constructor>, 1>> ClassAny::getDeclaredConstructors() const {
        return JXX_NEW<JxxArray<jxx::Ptr<Constructor>, 1>>(0);
    }
    jxx::Ptr<Constructor> ClassAny::getConstructor(jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> /*pt*/) const { return nullptr; }
    jxx::Ptr<Constructor> ClassAny::getDeclaredConstructor(jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> /*pt*/) const { return nullptr; }

    jxx::Ptr<JxxArray<jxx::Ptr<Annotation>, 1>> ClassAny::getAnnotations() const {
        return JXX_NEW<JxxArray<jxx::Ptr<Annotation>, 1>>(0);
    }
    jxx::Ptr<JxxArray<jxx::Ptr<Annotation>, 1>> ClassAny::getDeclaredAnnotations() const {
        return JXX_NEW<JxxArray<jxx::Ptr<Annotation>, 1>>(0);
    }

    jxx::Ptr<Package> ClassAny::getPackage() const { return nullptr; }
    jxx::Ptr<ClassLoader> ClassAny::getClassLoader() const { return nullptr; }

} // namespace jxx::lang