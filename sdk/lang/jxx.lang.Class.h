#pragma once
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <functional>
#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Object.h"
#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.ClassInfoMarker.h"

namespace jxx::lang {

    // Forward declarations for reflection types (optional / stubbed)
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
    class ClassAny final : public Object, public virtual jxx::io::Serializable {
    public:
        // ---------------------------------------------------------------------
        // Metadata structure (runtime reflection substrate)
        // ---------------------------------------------------------------------
        using FactoryFn = std::function<jxx::Ptr<Object>()>;

        using InstancePredicate = std::function<jbool(const jxx::Ptr<Object>&)>;

        struct Meta
        {
            std::string binaryName;

            std::type_index typeId =
                typeid(void);

            jxx::Ptr<ClassAny> superClass;

            std::vector<
                jxx::Ptr<ClassAny>>
                interfaces;

            jbool isInterface = false;
            jbool isPrimitive = false;
            jbool isArray = false;
            jbool isEnum = false;
            jbool isAnnotation = false;
            jbool isSynthetic = false;

            jxx::Ptr<ClassAny>
                componentType;

            std::weak_ptr<ClassLoader>
                classLoader;

            jint modifiers = 0;

            FactoryFn factory;

            /**
             * Runtime equivalent of Java Class.isInstance().
             *
             * For an Object-derived JXX class, this performs a normal
             * dynamic cast to that class.
             *
             * For a pure-virtual JXX interface, this performs a cross-cast
             * from Object through the complete concrete implementation.
             */
            InstancePredicate
                instancePredicate;
        };

        explicit ClassAny(Meta meta);

        // ---------------------------------------------------------------------
        // Registry / lookup
        // ---------------------------------------------------------------------
        static jxx::Ptr<ClassAny> registerClass(const Meta& meta);

        // Look up by Java name (binary name)
        static jxx::Ptr<ClassAny> forName(const jxx::Ptr<String>& className);

        // Java overloads exist in Java 8; we keep signature parity but ignore loader/initialize
        static jxx::Ptr<ClassAny> forName(const jxx::Ptr<String>& className, jbool initialize, const jxx::Ptr<ClassLoader>& loader);

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
        jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<ClassAny>, 1U>> getInterfaces() const;
        jxx::Ptr<ClassAny> getComponentType() const;
        jxx::Ptr<ClassAny> getDeclaringClass() const;
        jxx::Ptr<ClassAny> getEnclosingClass() const;
        jbool isMemberClass() const;

        jbool isAssignableFrom(const jxx::Ptr<ClassAny>& cls) const;
        jbool isInstance(const jxx::Ptr<Object>& obj) const;
        jxx::Ptr<Object> cast(const jxx::Ptr<Object>& obj) const;

        jxx::Ptr<ClassAny> asSubclass(
            const jxx::Ptr<ClassAny>& clazz) const;

        jbool desiredAssertionStatus() const;

        // Java 8 (deprecated later): newInstance()
        jxx::Ptr<Object> newInstance() const;

        // toString() in Java returns "class X" or "interface X"
        jxx::Ptr<String> toString() const override;

        // ---------------------------------------------------------------------
        // Array class helper (creates/returns canonical array ClassAny)
        // ---------------------------------------------------------------------
        static jxx::Ptr<ClassAny> arrayOf(const jxx::Ptr<ClassAny>& component);

        // Class-level annotation and loading information.
        jxx::Ptr<JxxArray<jxx::Ptr<Annotation>, 1>> getAnnotations() const;
        jxx::Ptr<JxxArray<jxx::Ptr<Annotation>, 1>> getDeclaredAnnotations() const;

        jxx::Ptr<Package> getPackage() const;
        jxx::Ptr<ClassLoader> getClassLoader() const;

        // ---------------------------------------------------------------------
        // JXX helpers
        // ---------------------------------------------------------------------
        const Meta& meta() const noexcept { return meta_; }

        //void writeObject(const jxx::Ptr<jxx::io::ObjectOutputStream>& out);
       // void readObject(const jxx::Ptr<jxx::io::ObjectInputStream>& in);
        //void readObjectNoData();

    private:
        friend class ClassLoader;

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
