#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Package.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.ClassNotFoundException.h"
#include "lang/jxx.lang.ClassCastException.h"
#include "lang/jxx.lang.ClassLoader.h"
#include "lang/jxx.lang.InstantiationException.h"
#include "lang/jxx.lang.IllegalAccessException.h"

#include <algorithm>

namespace jxx::lang {

    std::mutex ClassAny::registryMutex_{};
    std::unordered_map<std::string, std::weak_ptr<ClassAny>> ClassAny::registryByName_{};
    std::unordered_map<std::type_index, std::weak_ptr<ClassAny>> ClassAny::registryByType_{};

    ClassAny::ClassAny(Meta meta) : meta_(std::move(meta)) {}


    jxx::Ptr<ClassAny> ClassAny::registerClass(const Meta& meta) {
        if (meta.binaryName.empty()) {
            throw IllegalArgumentException(jxx::NEW<String>("Class.registerClass: binaryName is empty"));
        }

        std::lock_guard<std::mutex> lk(registryMutex_);

        // Return existing canonical instance if still alive
        if (auto it = registryByName_.find(meta.binaryName); it != registryByName_.end()) {
            if (auto existing = it->second.lock()) return existing;
        }

        auto cls = jxx::NEW<ClassAny>(meta);

        registryByName_[meta.binaryName] = cls;
        if (meta.typeId != std::type_index(typeid(void))) {
            registryByType_[meta.typeId] = cls;
        }
        return cls;
    }

    jxx::Ptr<ClassAny> ClassAny::forName(const jxx::Ptr<String>& className)
    {

        if (className == nullptr) {
            throw NullPointerException(
                jxx::NEW<String>(
                    "className"));
        }

        const std::string key =
            className->utf8();

        std::lock_guard<std::mutex>
            lock(registryMutex_);

        auto iterator =
            registryByName_.find(key);

        if (iterator !=
            registryByName_.end()) {

            if (auto descriptor =
                    iterator->
                        second.lock()) {

                return descriptor;
            }
        }

        throw ClassNotFoundException(
            jxx::NEW<String>(key));
    }

    jxx::Ptr<ClassAny> ClassAny::forName(
        const jxx::Ptr<String>& className,
        jbool /* initialize */,
        const jxx::Ptr<ClassLoader>& loader)
    {
        return loader == nullptr
            ? forName(className)
            : loader->loadClass(className);
    }

    jxx::Ptr<ClassAny> ClassAny::forType(const std::type_index& tid) {
        std::lock_guard<std::mutex> lk(registryMutex_);
        auto it = registryByType_.find(tid);
        if (it != registryByType_.end()) {
            if (auto c = it->second.lock()) return c;
        }

        // Java never returns null; if type isn't registered, this is a runtime configuration error.
        throw IllegalStateException(jxx::NEW<String>("Class.forType: unregistered type"));
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
        return jxx::NEW<String>(meta_.binaryName.c_str());
    }

    std::string ClassAny::simpleNameFromBinary_(
        const std::string& binaryName) {
        const auto packageSeparator =
            binaryName.find_last_of('.');
        const auto nestedSeparator =
            binaryName.find_last_of('$');

        const auto separator =
            packageSeparator == std::string::npos
                ? nestedSeparator
                : nestedSeparator == std::string::npos
                      ? packageSeparator
                      : std::max(
                            packageSeparator,
                            nestedSeparator);

        return separator == std::string::npos
            ? binaryName
            : binaryName.substr(separator + 1U);
    }

    jxx::Ptr<String> ClassAny::getSimpleName() const {
        if (meta_.isArray && meta_.componentType != nullptr) {
            const auto componentName =
                meta_.componentType->getSimpleName();
            return jxx::NEW<String>(
                componentName->utf8() + "[]");
        }

        return jxx::NEW<String>(
            simpleNameFromBinary_(meta_.binaryName));
    }

    jxx::Ptr<String> ClassAny::getCanonicalName() const {
        if (meta_.isArray && meta_.componentType != nullptr) {
            const auto componentName =
                meta_.componentType->getCanonicalName();
            if (componentName == nullptr) {
                return nullptr;
            }
            return jxx::NEW<String>(
                componentName->utf8() + "[]");
        }

        return getName();
    }

    jxx::Ptr<String> ClassAny::getTypeName() const {
        if (meta_.isArray && meta_.componentType != nullptr) {
            const auto componentName =
                meta_.componentType->getTypeName();
            return jxx::NEW<String>(
                componentName->utf8() + "[]");
        }

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
        auto a = jxx::NEW<JxxArray<jxx::Ptr<ClassAny>, 1>>((std::uint32_t)v.size());
        for (std::uint32_t i = 0; i < (std::uint32_t)v.size(); ++i) (*a)[(jint)i] = v[i];
        return a;
    }

    jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> ClassAny::getInterfaces() const {
        return toClassArray_(meta_.interfaces);
    }

    jxx::Ptr<ClassAny> ClassAny::getComponentType() const {
        return meta_.componentType;
    }

    jxx::Ptr<ClassAny> ClassAny::getDeclaringClass() const {
        if (meta_.isArray || meta_.isPrimitive) {
            return nullptr;
        }
        const auto separator = meta_.binaryName.find_last_of('$');
        if (separator == std::string::npos || separator == 0U) {
            return nullptr;
        }
        const auto name = meta_.binaryName.substr(0U, separator);
        std::lock_guard<std::mutex> lock(registryMutex_);
        const auto found = registryByName_.find(name);
        return found == registryByName_.end()
            ? nullptr
            : found->second.lock();
    }

    jxx::Ptr<ClassAny> ClassAny::getEnclosingClass() const {
        return getDeclaringClass();
    }

    jbool ClassAny::isMemberClass() const {
        return getDeclaringClass() != nullptr;
    }

    // Java array assignability special cases:
    // Any array is assignable to Object, Cloneable, Serializable.
    static bool isArraySuperInterfaceName(const std::string& n) {
        return (n == "java.lang.Object" || n == "java.lang.Cloneable" || n == "java.io.Serializable");
    }

    jbool ClassAny::isAssignableFromImpl_(const ClassAny& target, const ClassAny& source)
    {

        const auto voidType =
            std::type_index(
                typeid(void));

        /*
         * Same concrete RTTI type.
         *
         * Do not compare typeid(void), because array descriptors use that
         * as their sentinel type.
         */
        if (target.meta_.typeId != voidType &&
            source.meta_.typeId != voidType &&
            target.meta_.typeId ==
                source.meta_.typeId) {

            return true;
        }

        /*
         * Same canonical binary name.
         */
        if (!target.meta_.binaryName.empty() &&
            target.meta_.binaryName ==
                source.meta_.binaryName) {

            return true;
        }

        /*
         * Java array superclass/interface rules.
         */
        if (source.meta_.isArray &&
            isArraySuperInterfaceName(
                target.meta_.binaryName)) {

            return true;
        }

        /*
         * Java reference-array covariance:
         *
         * S[] is assignable to T[] when S is assignable to T.
         */
        if (target.meta_.isArray &&
            source.meta_.isArray) {

            const auto targetComponent =
                target.meta_.componentType;

            const auto sourceComponent =
                source.meta_.componentType;

            if (targetComponent == nullptr ||
                sourceComponent == nullptr) {

                return false;
            }

            /*
             * Primitive arrays are assignable only when their exact array
             * types match. Exact matches were handled above.
             */
            if (targetComponent->
                    meta_.isPrimitive ||
                sourceComponent->
                    meta_.isPrimitive) {

                return false;
            }

            return isAssignableFromImpl_(
                *targetComponent,
                *sourceComponent);
        }

        /*
         * Search every directly declared interface.
         *
         * Recursion also searches parent interfaces.
         */
        for (const auto& interfaceType :
             source.meta_.interfaces) {

            if (interfaceType == nullptr) {
                continue;
            }

            if (isAssignableFromImpl_(
                target,
                *interfaceType)) {

                return true;
            }
        }

        /*
         * Search the superclass recursively.
         *
         * This is the important correction. The recursive call checks:
         *
         *   - the superclass itself
         *   - interfaces implemented by the superclass
         *   - interfaces extended by those interfaces
         *   - the superclass's own superclass
         *
         * In this test:
         *
         *   ConcreteDevice
         *       -> AbstractDevice
         *           -> Startable
         */
        if (source.meta_.superClass != nullptr) {
            return isAssignableFromImpl_(
                target,
                *source.meta_.superClass);
        }

        return false;
    }

    jbool ClassAny::isAssignableFrom(const jxx::Ptr<ClassAny>& cls) const {
        if (!cls) throw NullPointerException(jxx::NEW<String>("cls"));
        return isAssignableFromImpl_(*this, *cls);
    }

    jbool ClassAny::isInstance(const jxx::Ptr<Object>& obj) const {
        if (!obj) return false;
        auto oc = obj->getClass();
        if (!oc) return false;
        return this->isAssignableFrom(oc);
    }

    jxx::Ptr<Object> ClassAny::cast(const jxx::Ptr<Object>& object) const
    {
        if (object == nullptr) {
            return nullptr;
        }

        if (isInstance(object)) {
            return object;
        }

        auto sourceClass =
            object->getClass();

        std::string message =
            sourceClass == nullptr
            ? "Object"
            : sourceClass->
            meta_.binaryName;

        message +=
            " cannot be cast to ";

        message +=
            meta_.binaryName;

        throw ClassCastException(
            jxx::NEW<String>(
                message));
    }

    jxx::Ptr<ClassAny> ClassAny::asSubclass(
        const jxx::Ptr<ClassAny>& clazz) const {
        if (clazz == nullptr) {
            throw NullPointerException(
                jxx::NEW<String>("clazz"));
        }

        auto thisClass = forName(getName());
        if (!clazz->isAssignableFrom(thisClass)) {
            throw ClassCastException(
                jxx::NEW<String>(
                    meta_.binaryName +
                    " cannot be cast to " +
                    clazz->meta_.binaryName));
        }

        return thisClass;
    }

    jbool ClassAny::desiredAssertionStatus() const {
        return false;
    }

    jxx::Ptr<Object> ClassAny::newInstance() const {
        // Java-ish checks
        if (meta_.isInterface || meta_.isPrimitive || meta_.isArray) {
            throw InstantiationException(jxx::NEW<String>(meta_.binaryName.c_str()));
        }
        // Modifier.ABSTRACT in Java is 0x0400
        constexpr jint ABSTRACT = 0x0400;
        if ((meta_.modifiers & ABSTRACT) != 0) {
            throw InstantiationException(jxx::NEW<String>(meta_.binaryName.c_str()));
        }
        if (!meta_.factory) {
            throw InstantiationException(jxx::NEW<String>(meta_.binaryName.c_str()));
        }
        return meta_.factory();
    }

    jxx::Ptr<String> ClassAny::toString() const {
        std::string value;
        if (!meta_.isPrimitive) {
            value = meta_.isInterface
                ? "interface "
                : "class ";
        }
        value += meta_.binaryName;
        return jxx::NEW<String>(value);
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

    jxx::Ptr<ClassAny> ClassAny::arrayOf(const jxx::Ptr<ClassAny>& component) {
        if (!component) throw NullPointerException(jxx::NEW<String>("component"));

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
        try { m.superClass = forName(jxx::NEW<String>("java.lang.Object")); }
        catch (...) { m.superClass = nullptr; }
        try { m.interfaces.push_back(forName(jxx::NEW<String>("java.lang.Cloneable"))); }
        catch (...) {}
        try { m.interfaces.push_back(forName(jxx::NEW<String>("java.io.Serializable"))); }
        catch (...) {}

        // No newInstance() for array Class in Java via Class.newInstance()
        m.factory = {};

        return registerClass(m);
    }

    jxx::Ptr<JxxArray<jxx::Ptr<Annotation>, 1>> ClassAny::getAnnotations() const {
        return jxx::NEW<JxxArray<jxx::Ptr<Annotation>, 1>>(0);
    }
    jxx::Ptr<JxxArray<jxx::Ptr<Annotation>, 1>> ClassAny::getDeclaredAnnotations() const {
        return jxx::NEW<JxxArray<jxx::Ptr<Annotation>, 1>>(0);
    }

    jxx::Ptr<Package> ClassAny::getPackage() const {
        if (meta_.isArray) {
            return meta_.componentType == nullptr
                ? nullptr
                : meta_.componentType->getPackage();
        }

        if (meta_.isPrimitive) {
            return nullptr;
        }

        const auto separator =
            meta_.binaryName.find_last_of('.');
        if (separator == std::string::npos) {
            return nullptr;
        }

        return Package::definePackage(
            jxx::NEW<String>(
                meta_.binaryName.substr(0U, separator)));
    }
    jxx::Ptr<ClassLoader> ClassAny::getClassLoader() const {
        if (meta_.isArray) {
            return meta_.componentType == nullptr
                ? nullptr
                : meta_.componentType->getClassLoader();
        }
        if (meta_.isPrimitive) {
            return nullptr;
        }
        return meta_.classLoader.lock();
    }
    /*
    void ClassAny::writeObject(const jxx::Ptr<jxx::io::ObjectOutputStream>& out) {
    }
    
        void ClassAny::readObject(const jxx::Ptr<jxx::io::ObjectInputStream>& in) {

    }
        void ClassAny::readObjectNoData() {

    }*/
} // namespace jxx::lang