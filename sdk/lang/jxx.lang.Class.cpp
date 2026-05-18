#include "jxx.lang.String.h"
#include "jxx.lang.Class.h"

#include <algorithm>

namespace jxx::lang {

    std::mutex ClassAny::registryMutex_{};
    std::unordered_map<std::string, std::weak_ptr<ClassAny>> ClassAny::registryByName_{};
    std::unordered_map<std::type_index, std::weak_ptr<ClassAny>> ClassAny::registryByType_{};

    ClassAny::ClassAny(Meta meta) : meta_(std::move(meta)) {}

    jxx::Ptr<ClassAny> ClassAny::registerClass(const Meta& meta) {
        if (meta.binaryName.empty()) {
            throw IllegalArgumentException(JXX_NEW<String>("Class.registerClass: binaryName is empty"));
        }

        std::lock_guard<std::mutex> lk(registryMutex_);

        // Return existing canonical instance if still alive
        if (auto it = registryByName_.find(meta.binaryName); it != registryByName_.end()) {
            if (auto existing = it->second.lock()) return existing;
        }

        auto cls = JXX_NEW<ClassAny>(meta);

        registryByName_[meta.binaryName] = cls;
        if (meta.typeId != std::type_index(typeid(void))) {
            registryByType_[meta.typeId] = cls;
        }
        return cls;
    }

    jxx::Ptr<ClassAny> ClassAny::forName(jxx::Ptr<String> className) {
        if (!className) throw NullPointerException(JXX_NEW<String>("className"));

        std::string key = className->utf8();
        std::lock_guard<std::mutex> lk(registryMutex_);

        auto it = registryByName_.find(key);
        if (it != registryByName_.end()) {
            if (auto c = it->second.lock()) return c;
        }

        throw ClassNotFoundException(JXX_NEW<String>(className->utf8().c_str()));
    }

    jxx::Ptr<ClassAny> ClassAny::forName(jxx::Ptr<String> className, jxx::lang::jbool /*initialize*/, jxx::Ptr<ClassLoader> /*loader*/) {
        // Java allows loading via specific loader and optional initialization.
        // JXX runtime typically has a single registry, so we treat as alias.
        return forName(className);
    }

    jxx::Ptr<ClassAny> ClassAny::forType(const std::type_index& tid) {
        std::lock_guard<std::mutex> lk(registryMutex_);
        auto it = registryByType_.find(tid);
        if (it != registryByType_.end()) {
            if (auto c = it->second.lock()) return c;
        }

        // Java never returns null; if type isn't registered, this is a runtime configuration error.
        throw IllegalStateException(JXX_NEW<String>("Class.forType: unregistered type"));
    }

    jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> ClassAny::getAllLoadedClasses() {
        std::lock_guard<std::mutex> lk(registryMutex_);
        std::vector<jxx::Ptr<ClassAny>> alive;
        alive.reserve(registryByName_.size());
        for (auto& kv : registryByName_) {
            if (auto c = kv.second.lock()) alive.push_back(c);
        }
        return toClassArray_(alive);
    }

    jxx::Ptr<String> ClassAny::getName() const {
        return JXX_NEW<String>(meta_.binaryName.c_str());
    }

    std::string ClassAny::simpleNameFromBinary_(const std::string& bin) {
        // For array binary names ([I, [Ljava.lang.String;), Java simple name is complicated.
        // We return a practical approximation:
        //  - For normal names, substring after last '.'
        //  - For arrays, use component simple name + "[]"
        if (!bin.empty() && bin[0] == '[') {
            // decode one-dimensional for display
            if (bin.size() >= 2 && bin[1] != 'L') {
                // primitive array code
                return std::string(1, bin[1]) + "[]";
            }
            // ref array: "[Lpkg.Type;"
            auto semi = bin.find(';');
            auto inner = (semi == std::string::npos) ? bin : bin.substr(2, semi - 2);
            auto pos = inner.find_last_of('.');
            auto leaf = (pos == std::string::npos) ? inner : inner.substr(pos + 1);
            return leaf + "[]";
        }

        auto pos = bin.find_last_of('.');
        if (pos == std::string::npos) return bin;
        return bin.substr(pos + 1);
    }

    jxx::Ptr<String> ClassAny::getSimpleName() const {
        return JXX_NEW<String>(simpleNameFromBinary_(meta_.binaryName).c_str());
    }

    jxx::Ptr<String> ClassAny::getCanonicalName() const {
        // Java returns null for anonymous/local classes. If you track that, change here.
        return getName();
    }

    jxx::Ptr<String> ClassAny::getTypeName() const {
        // Java 8: for Class, getTypeName equals getName for most cases.
        return getName();
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
        for (std::uint32_t i = 0; i < (std::uint32_t)v.size(); ++i) (*a)[(jint)i] = v[i];
        return a;
    }

    jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> ClassAny::getInterfaces() const {
        return toClassArray_(meta_.interfaces);
    }

    jxx::Ptr<ClassAny> ClassAny::getComponentType() const {
        return meta_.componentType;
    }

    // Java array assignability special cases:
    // Any array is assignable to Object, Cloneable, Serializable.
    static bool isArraySuperInterfaceName(const std::string& n) {
        return (n == "java.lang.Object" || n == "java.lang.Cloneable" || n == "java.io.Serializable");
    }

    jbool ClassAny::isAssignableFromImpl_(const ClassAny& target, const ClassAny& src) {
        // same type
        if (target.meta_.typeId == src.meta_.typeId) return true;
        if (!target.meta_.binaryName.empty() && target.meta_.binaryName == src.meta_.binaryName) return true;

        // array special rules
        if (src.meta_.isArray && isArraySuperInterfaceName(target.meta_.binaryName)) return true;

        // superclass chain
        for (auto cur = src.meta_.superClass; cur; cur = cur->meta_.superClass) {
            if (cur->meta_.typeId == target.meta_.typeId) return true;
            if (cur->meta_.binaryName == target.meta_.binaryName) return true;
        }

        // interface closure
        std::vector<jxx::Ptr<ClassAny>> stack(src.meta_.interfaces.begin(), src.meta_.interfaces.end());
        while (!stack.empty()) {
            auto top = stack.back();
            stack.pop_back();
            if (!top) continue;
            if (top->meta_.typeId == target.meta_.typeId) return true;
            if (top->meta_.binaryName == target.meta_.binaryName) return true;
            for (auto& ii : top->meta_.interfaces) stack.push_back(ii);
        }

        // array component covariance:
        // In Java, S[] is assignable to T[] if S is assignable to T and both are reference types.
        if (target.meta_.isArray && src.meta_.isArray) {
            auto tc = target.meta_.componentType;
            auto sc = src.meta_.componentType;
            if (!tc || !sc) return false;
            if (tc->meta_.isPrimitive || sc->meta_.isPrimitive) {
                // primitive arrays only assignable if same primitive type (handled above)
                return false;
            }
            return isAssignableFromImpl_(*tc, *sc);
        }

        return false;
    }

    jbool ClassAny::isAssignableFrom(jxx::Ptr<ClassAny> cls) const {
        if (!cls) throw NullPointerException(JXX_NEW<String>("cls"));
        return isAssignableFromImpl_(*this, *cls);
    }

    jbool ClassAny::isInstance(jxx::Ptr<Object> obj) const {
        if (!obj) return false;
        auto oc = obj->getClass();
        if (!oc) return false;
        return this->isAssignableFrom(oc);
    }

    jxx::Ptr<Object> ClassAny::cast(jxx::Ptr<Object> obj) const {
        if (!obj) return nullptr;
        if (isInstance(obj)) return obj;

        auto src = obj->getClass();
        std::string msg;
        if (src) msg = src->meta_.binaryName;
        else msg = "Object";
        msg += " cannot be cast to ";
        msg += meta_.binaryName;
        throw ClassCastException(JXX_NEW<String>(msg.c_str()));
    }

    jxx::Ptr<Object> ClassAny::newInstance() const {
        // Java-ish checks
        if (meta_.isInterface || meta_.isPrimitive || meta_.isArray) {
            throw InstantiationException(JXX_NEW<String>(meta_.binaryName.c_str()));
        }
        // Modifier.ABSTRACT in Java is 0x0400
        constexpr jint ABSTRACT = 0x0400;
        if ((meta_.modifiers & ABSTRACT) != 0) {
            throw InstantiationException(JXX_NEW<String>(meta_.binaryName.c_str()));
        }
        if (!meta_.factory) {
            throw InstantiationException(JXX_NEW<String>(meta_.binaryName.c_str()));
        }
        return meta_.factory();
    }

    jxx::Ptr<String> ClassAny::toString() const {
        std::string s = meta_.isInterface ? "interface " : "class ";
        s += meta_.binaryName;
        return JXX_NEW<String>(s.c_str());
    }

    // -------- Array class creation helpers --------
    char ClassAny::primitiveArrayCode_(const ClassAny& prim) {
        // Java array codes: Z B C S I J F D
        const auto& n = prim.meta_.binaryName;
        if (n == "boolean") return 'Z';
        if (n == "byte")    return 'B';
        if (n == "char")    return 'C';
        if (n == "short")   return 'S';
        if (n == "int")     return 'I';
        if (n == "long")    return 'J';
        if (n == "float")   return 'F';
        if (n == "double")  return 'D';
        return '?';
    }

    std::string ClassAny::arrayBinaryName_(const ClassAny& component) {
        // Java binary name rules:
        //  - primitive component: "[" + code
        //  - ref component: "[L" + binaryName + ";"
        if (component.meta_.isPrimitive) {
            char code = primitiveArrayCode_(component);
            return std::string("[") + code;
        }
        return std::string("[L") + component.meta_.binaryName + ";";
    }

    jxx::Ptr<ClassAny> ClassAny::arrayOf(jxx::Ptr<ClassAny> component) {
        if (!component) throw NullPointerException(JXX_NEW<String>("component"));

        std::string name = arrayBinaryName_(*component);

        // return canonical array class if already present
        {
            std::lock_guard<std::mutex> lk(registryMutex_);
            auto it = registryByName_.find(name);
            if (it != registryByName_.end()) {
                if (auto existing = it->second.lock()) return existing;
            }
        }

        // create array Meta
        Meta m;
        m.binaryName = name;
        m.typeId = std::type_index(typeid(void)); // no unique C++ RTTI for arbitrary arrays in this model
        m.isArray = true;
        m.componentType = component;

        // Arrays are Objects and implement Cloneable + Serializable in Java.
        // If those classes exist and were registered, attach them.
        // (Safe: if not registered, ignore.)
        try { m.superClass = forName(JXX_NEW<String>("java.lang.Object")); }
        catch (...) { m.superClass = nullptr; }
        try { m.interfaces.push_back(forName(JXX_NEW<String>("java.lang.Cloneable"))); }
        catch (...) {}
        try { m.interfaces.push_back(forName(JXX_NEW<String>("java.io.Serializable"))); }
        catch (...) {}

        // No newInstance() for array Class in Java via Class.newInstance()
        m.factory = {};

        return registerClass(m);
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