#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "io/jxx.io.Serializable.h"

// Exceptions required (as you stated)
#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.IllegalArgumentException.h"
#include "jxx.lang.IllegalStateException.h"
#include "jxx.lang.ClassNotFoundException.h"
#include "jxx.lang.ClassCastException.h"
#include "jxx.lang.InstantiationException.h"
#include "jxx.lang.IllegalAccessException.h"

#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <functional>

namespace jxx::lang {

    // Forward declarations for reflection types (optional / stubbed)
    class Field;
    class Method;
    class Constructor;
    class Annotation;
    class Package;
    class ClassLoader;
    class String;

    /**
     * Java 8 parity: java.lang.Class<T>
     *
     * In JXX we provide type-erased ClassAny, and use it everywhere
     * a Java Class<?> would appear. (Your requirement)
     */
    class ClassAny final : public Object, public jxx::io::Serializable {
    public:
        // ---------------------------------------------------------------------
        // Metadata structure (runtime reflection substrate)
        // ---------------------------------------------------------------------
        using FactoryFn = std::function<jxx::Ptr<Object>()>;

        struct Meta {
            // Java binary name (Class.getName()):
            //  - normal: "java.lang.String"
            //  - array primitive: "[I", "[Z", etc
            //  - array ref: "[Ljava.lang.String;"
            std::string binaryName;

            // RTTI key for Object::getClass() -> ClassAny mapping
            std::type_index typeId = typeid(void);

            // super + interfaces
            jxx::Ptr<ClassAny> superClass;                 // null for Object / primitive / void
            std::vector<jxx::Ptr<ClassAny>> interfaces;    // direct interfaces

            // kind flags
            jbool isInterface = false;
            jbool isPrimitive = false;
            jbool isArray = false;
            jbool isEnum = false;
            jbool isAnnotation = false;
            jbool isSynthetic = false;

            // arrays
            jxx::Ptr<ClassAny> componentType;              // for arrays only

            // modifier bitmask (java.lang.reflect.Modifier compatible)
            jint modifiers = 0;

            // Java 8 Class.newInstance() factory (public no-arg ctor)
            // If not set => newInstance throws InstantiationException.
            FactoryFn factory;
        };

        // ---------------------------------------------------------------------
        // Registry / lookup
        // ---------------------------------------------------------------------
        static jxx::Ptr<ClassAny> registerClass(const Meta& meta);

        // Look up by Java name (binary name)
        static jxx::Ptr<ClassAny> forName(jxx::Ptr<String> className);

        // Java overloads exist in Java 8; we keep signature parity but ignore loader/initialize
        static jxx::Ptr<ClassAny> forName(jxx::Ptr<String> className, jbool initialize, jxx::Ptr<ClassLoader> loader);

        // Look up by RTTI key (used by Object::getClass())
        static jxx::Ptr<ClassAny> forType(const std::type_index& tid);

        // Optional: return all registered classes (useful for debugging)
        static jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> getAllLoadedClasses();

        // ---------------------------------------------------------------------
        // Core Java 8 APIs (implemented)
        // ---------------------------------------------------------------------
        jxx::Ptr<String> getName() const;
        jxx::Ptr<String> getSimpleName() const;
        jxx::Ptr<String> getCanonicalName() const;
        jxx::Ptr<String> getTypeName() const;

        jint getModifiers() const;

        jbool isInterface() const;
        jbool isPrimitive() const;
        jbool isArray() const;
        jbool isEnum() const;
        jbool isAnnotation() const;
        jbool isSynthetic() const;

        jxx::Ptr<ClassAny> getSuperclass() const;
        jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> getInterfaces() const;
        jxx::Ptr<ClassAny> getComponentType() const;

        jbool isAssignableFrom(jxx::Ptr<ClassAny> cls) const;
        jbool isInstance(jxx::Ptr<Object> obj) const;
        jxx::Ptr<Object> cast(jxx::Ptr<Object> obj) const;

        // Java 8 (deprecated later): newInstance()
        jxx::Ptr<Object> newInstance() const;

        // toString() in Java returns "class X" or "interface X"
        jxx::Ptr<String> toString() const override;

        // ---------------------------------------------------------------------
        // Array class helper (creates/returns canonical array ClassAny)
        // ---------------------------------------------------------------------
        static jxx::Ptr<ClassAny> arrayOf(jxx::Ptr<ClassAny> component);

        // ---------------------------------------------------------------------
        // Reflection-heavy Java 8 APIs (declared for parity; stubbed in .cpp)
        // ---------------------------------------------------------------------
        jxx::Ptr<JxxArray<jxx::Ptr<Field>, 1>> getFields() const;
        jxx::Ptr<JxxArray<jxx::Ptr<Field>, 1>> getDeclaredFields() const;
        jxx::Ptr<Field> getField(jxx::Ptr<String> name) const;
        jxx::Ptr<Field> getDeclaredField(jxx::Ptr<String> name) const;

        jxx::Ptr<JxxArray<jxx::Ptr<Method>, 1>> getMethods() const;
        jxx::Ptr<JxxArray<jxx::Ptr<Method>, 1>> getDeclaredMethods() const;
        jxx::Ptr<Method> getMethod(jxx::Ptr<String> name, jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> parameterTypes) const;
        jxx::Ptr<Method> getDeclaredMethod(jxx::Ptr<String> name, jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> parameterTypes) const;

        jxx::Ptr<JxxArray<jxx::Ptr<Constructor>, 1>> getConstructors() const;
        jxx::Ptr<JxxArray<jxx::Ptr<Constructor>, 1>> getDeclaredConstructors() const;
        jxx::Ptr<Constructor> getConstructor(jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> parameterTypes) const;
        jxx::Ptr<Constructor> getDeclaredConstructor(jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> parameterTypes) const;

        jxx::Ptr<JxxArray<jxx::Ptr<Annotation>, 1>> getAnnotations() const;
        jxx::Ptr<JxxArray<jxx::Ptr<Annotation>, 1>> getDeclaredAnnotations() const;

        jxx::Ptr<Package> getPackage() const;
        jxx::Ptr<ClassLoader> getClassLoader() const;

        // ---------------------------------------------------------------------
        // JXX helpers
        // ---------------------------------------------------------------------
        const Meta& meta() const noexcept { return meta_; }

    private:
        explicit ClassAny(Meta meta);

        Meta meta_;

        // canonical registry
        static std::mutex registryMutex_;
        static std::unordered_map<std::string, std::weak_ptr<ClassAny>> registryByName_;
        static std::unordered_map<std::type_index, std::weak_ptr<ClassAny>> registryByType_;

        // helpers
        static jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> toClassArray_(const std::vector<jxx::Ptr<ClassAny>>& v);
        static std::string simpleNameFromBinary_(const std::string& bin);
        static jbool isAssignableFromImpl_(const ClassAny& target, const ClassAny& src);
        static std::string arrayBinaryName_(const ClassAny& component);
        static char primitiveArrayCode_(const ClassAny& prim);
    };

    // As requested: "Class<?> will become ClassAny"
    using Class = ClassAny;

} // namespace jxx::lang
