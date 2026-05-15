#pragma once
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <functional>
#include <algorithm>

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "io/jxx.io.Serializable.h"

#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.IllegalArgumentException.h"

// If you have these, include them; otherwise forward declare and adjust throws
#include "jxx.lang.ClassNotFoundException.h"
#include "jxx.lang.ClassCastException.h"
#include "jxx.lang.InstantiationException.h"
#include "jxx.lang.IllegalAccessException.h"


namespace jxx::lang {

    // Forward declarations for reflection types (stubs for now)
    class Field;
    class Method;
    class Constructor;
    class Annotation;
    class Package;
    class ClassLoader;
    class String;

    /**
     * Java 8: public final class Class<T>
     *
     * In JXX: we implement a single runtime type-erased class object ClassAny.
     * Generic uses in transpiled code can map to ClassAny as needed.
     *
     * As requested: Class<?> becomes ClassAny.
     */
    class ClassAny final : public Object, public jxx::io::Serializable {
    public:
        // -----------------------------------------
        // Registration / lookup (JXX runtime support)
        // -----------------------------------------

        using FactoryFn = std::function<jxx::Ptr<Object>()>;

        struct Meta {
            std::string binaryName;                 // e.g. "java.lang.String"
            std::type_index typeId = typeid(void);  // C++ RTTI key (if available)
            jxx::Ptr<ClassAny> superClass;          // nullptr for Object / primitives
            std::vector<jxx::Ptr<ClassAny>> interfaces;
            jbool isInterface = false;
            jbool isPrimitive = false;
            jbool isArray = false;
            jbool isEnum = false;
            jbool isAnnotation = false;
            jbool isSynthetic = false;
            jxx::Ptr<ClassAny> componentType;       // for arrays
            jint modifiers = 0;                     // Java modifier bitmask
            FactoryFn factory;                      // for newInstance()
        };

        /**
         * Register a class metadata object with the global registry.
         * Returns the canonical ClassAny instance stored in the registry.
         */
        static jxx::Ptr<ClassAny> registerClass(const Meta& meta);

        /**
         * Java: static Class<?> forName(String className)
         */
        static jxx::Ptr<ClassAny> forName(jxx::Ptr<String> className);

        /**
         * Java: String getName()
         */
        jxx::Ptr<String> getName() const;

        /**
         * Java: String getSimpleName()
         */
        jxx::Ptr<String> getSimpleName() const;

        /**
         * Java: String getCanonicalName()
         * (For parity: returns same as name for most classes; null for anonymous/local)
         */
        jxx::Ptr<String> getCanonicalName() const;

        /**
         * Java: String toString()
         * "class X" or "interface X"
         */
        jxx::Ptr<String> toString() const override;

        /**
         * Java: int getModifiers()
         */
        jint getModifiers() const;

        /**
         * Java: boolean isInterface(), isPrimitive(), isArray(), isEnum(), isAnnotation(), isSynthetic()
         */
        jbool isInterface() const;
        jbool isPrimitive() const;
        jbool isArray() const;
        jbool isEnum() const;
        jbool isAnnotation() const;
        jbool isSynthetic() const;

        /**
         * Java: Class<?> getSuperclass()
         */
        jxx::Ptr<ClassAny> getSuperclass() const;

        /**
         * Java: Class<?>[] getInterfaces()
         */
        jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> getInterfaces() const;

        /**
         * Java: Class<?> getComponentType() (arrays)
         */
        jxx::Ptr<ClassAny> getComponentType() const;

        /**
         * Java: boolean isAssignableFrom(Class<?> cls)
         */
        jbool isAssignableFrom(jxx::Ptr<ClassAny> cls) const;

        /**
         * Java: boolean isInstance(Object obj)
         */
        jbool isInstance(jxx::Ptr<Object> obj) const;

        /**
         * Java: T cast(Object obj)  (here: Object cast(Object))
         */
        jxx::Ptr<Object> cast(jxx::Ptr<Object> obj) const;

        /**
         * Java: T newInstance()
         * (Deprecated in newer Java but present in Java 8)
         */
        jxx::Ptr<Object> newInstance() const;

        // -----------------------------------------
        // Reflection-heavy APIs (Java 8) - stubs
        // -----------------------------------------

        // Fields
        jxx::Ptr<JxxArray<jxx::Ptr<Field>, 1>> getFields() const;
        jxx::Ptr<JxxArray<jxx::Ptr<Field>, 1>> getDeclaredFields() const;
        jxx::Ptr<Field> getField(jxx::Ptr<String> name) const;
        jxx::Ptr<Field> getDeclaredField(jxx::Ptr<String> name) const;

        // Methods
        jxx::Ptr<JxxArray<jxx::Ptr<Method>, 1>> getMethods() const;
        jxx::Ptr<JxxArray<jxx::Ptr<Method>, 1>> getDeclaredMethods() const;
        jxx::Ptr<Method> getMethod(jxx::Ptr<String> name, jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> parameterTypes) const;
        jxx::Ptr<Method> getDeclaredMethod(jxx::Ptr<String> name, jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> parameterTypes) const;

        // Constructors
        jxx::Ptr<JxxArray<jxx::Ptr<Constructor>, 1>> getConstructors() const;
        jxx::Ptr<JxxArray<jxx::Ptr<Constructor>, 1>> getDeclaredConstructors() const;
        jxx::Ptr<Constructor> getConstructor(jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> parameterTypes) const;
        jxx::Ptr<Constructor> getDeclaredConstructor(jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> parameterTypes) const;

        // Annotations (stubs)
        jxx::Ptr<JxxArray<jxx::Ptr<Annotation>, 1>> getAnnotations() const;
        jxx::Ptr<JxxArray<jxx::Ptr<Annotation>, 1>> getDeclaredAnnotations() const;

        // Package / ClassLoader (stubs)
        jxx::Ptr<Package> getPackage() const;
        jxx::Ptr<ClassLoader> getClassLoader() const;

        // -----------------------------------------
        // JXX helper
        // -----------------------------------------
        const Meta& meta() const noexcept { return meta_; }

    private:
        explicit ClassAny(Meta meta);

        Meta meta_;

        // Registry storage
        static std::mutex registryMutex_;
        static std::unordered_map<std::string, std::weak_ptr<ClassAny>> registry_;

        // helpers
        static jxx::Ptr<JxxArray<jxx::Ptr<ClassAny>, 1>> toClassArray_(const std::vector<jxx::Ptr<ClassAny>>& v);
        static std::string simpleNameFromBinary_(const std::string& bin);
        static jbool isAssignableFromImpl_(const ClassAny& target, const ClassAny& src);
    };

    // As requested: "static Class<?> will become ClassAny"
    using Class = ClassAny;

} // namespace jxx::lang