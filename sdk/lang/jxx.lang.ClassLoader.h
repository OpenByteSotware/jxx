#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"
#include "jxx.lang.Class.h"
#include "jxx.lang.Package.h"


#include "util/jxx.util.Enumeration.h"
#include "net/jxx.net.URL.h"

#include <unordered_map>
#include <vector>
#include <mutex>
#include <string>

namespace jxx::io { class InputStream; }

namespace jxx::lang {

/**
 * Java 8 parity: java.lang.ClassLoader
 *
 * - Class type => inherits Object
 * - Parent-first delegation model
 * - Per-name loading lock (getClassLoadingLock)
 * - Loaded-class cache (findLoadedClass)
 * - Resource lookup (getResource/getResources)
 * - Assertion status APIs (setDefaultAssertionStatus, etc.)
 * - Package APIs: getPackage/getPackages + protected definePackage
 *
 * NOTE: Bytecode defineClass(...) is not supported in JXX by default.
 * Override findClass(...) in derived loaders to register classes into ClassAny registry.
 */
class ClassLoader : public Object {
public:

    ClassLoader();                           // parent = system
    explicit ClassLoader(jxx::Ptr<ClassLoader> parent);
    virtual ~ClassLoader() = default;

    // -------------------------
    // Java 8 public API
    // -------------------------

    jxx::Ptr<ClassLoader> getParent() const;

    static jxx::Ptr<ClassLoader> getSystemClassLoader();

    static jxx::Ptr<jxx::net::URL> getSystemResource(jxx::Ptr<String> name);
    static jxx::Ptr<jxx::util::Enumeration<jxx::Ptr<jxx::net::URL>>> getSystemResources(jxx::Ptr<String> name);
    static jxx::Ptr<jxx::io::InputStream> getSystemResourceAsStream(jxx::Ptr<String> name);

    jxx::Ptr<ClassAny> loadClass(jxx::Ptr<String> name);
    virtual jxx::Ptr<ClassAny> loadClass(jxx::Ptr<String> name, jbool resolve);

    virtual jxx::Ptr<jxx::net::URL> getResource(jxx::Ptr<String> name);
    virtual jxx::Ptr<jxx::util::Enumeration<jxx::Ptr<jxx::net::URL>>> getResources(jxx::Ptr<String> name);
    virtual jxx::Ptr<jxx::io::InputStream> getResourceAsStream(jxx::Ptr<String> name);

    // Assertions (Java 8)
    void setDefaultAssertionStatus(jbool enabled);
    void setPackageAssertionStatus(jxx::Ptr<String> packageName, jbool enabled);
    void setClassAssertionStatus(jxx::Ptr<String> className, jbool enabled);
    void clearAssertionStatus();
    jbool desiredAssertionStatus(jxx::Ptr<ClassAny> clazz) const;

    // Packages (Java 8)
    jxx::Ptr<Package> getPackage(jxx::Ptr<String> name);
    jxx::Ptr<JxxArray<jxx::Ptr<Package>, 1>> getPackages();

protected:
    // -------------------------
    // Java 8 protected API
    // -------------------------

   

    // Override point: provide actual class loading
    virtual jxx::Ptr<ClassAny> findClass(jxx::Ptr<String> name);

    // Link/resolve hook (no-op in JXX)
    virtual void resolveClass(jxx::Ptr<ClassAny> c);

    // Cache lookup (per loader)
    virtual jxx::Ptr<ClassAny> findLoadedClass(jxx::Ptr<String> name);

    // Bytecode defineClass - unsupported by default (override if you build a bytecode pipeline)
    virtual jxx::Ptr<ClassAny> defineClass(jxx::Ptr<String> name, jxx::Ptr<ByteArray> b, jint off, jint len);
    virtual jxx::Ptr<ClassAny> defineClass(jxx::Ptr<ByteArray> b, jint off, jint len);

    // Resource hooks
    virtual jxx::Ptr<jxx::net::URL> findResource(jxx::Ptr<String> name);
    virtual jxx::Ptr<jxx::util::Enumeration<jxx::Ptr<jxx::net::URL>>> findResources(jxx::Ptr<String> name);

    // System class lookup (bootstrap-ish). In JXX we forward to ClassAny registry.
    virtual jxx::Ptr<ClassAny> findSystemClass(jxx::Ptr<String> name);

    // Java 7+: per-name loading lock
    virtual jxx::Ptr<Object> getClassLoadingLock(jxx::Ptr<String> className);

    // Package definition helper
    virtual jxx::Ptr<Package> definePackage(jxx::Ptr<String> name);

    // Java 8 protected: registerAsParallelCapable()
    static jbool registerAsParallelCapable();

    // Java 8 protected: findLibrary(String libname) -> String
    virtual jxx::Ptr<String> findLibrary(jxx::Ptr<String> libname);

    // JXX-only: in-memory resource store
    void addResource(jxx::Ptr<String> name, jxx::Ptr<ByteArray> bytes);

private:
    jxx::Ptr<ClassLoader> parent_;

    // loaded classes cache: binaryName -> weak ClassAny
    mutable std::mutex loadedMutex_;
    std::unordered_map<std::string, std::weak_ptr<ClassAny>> loadedByName_;

    // per-class loading locks: name -> Object lock
    mutable std::mutex lockMapMutex_;
    std::unordered_map<std::string, std::weak_ptr<Object>> loadLocks_;

    // resource store: name -> bytes
    mutable std::mutex resourceMutex_;
    std::unordered_map<std::string, jxx::Ptr<ByteArray>> resources_;

    // assertion status
    jbool defaultAssertionStatus_ = false;
    mutable std::mutex assertionMutex_;
    std::unordered_map<std::string, jbool> classAssertion_;
    std::unordered_map<std::string, jbool> packageAssertion_;

    // packages
    mutable std::mutex pkgMutex_;
    std::unordered_map<std::string, std::weak_ptr<Package>> packages_;

    // system loader singleton
    static std::mutex systemMutex_;
    static std::weak_ptr<ClassLoader> systemLoader_;

    // Lock object used by getClassLoadingLock()
    class LoadingLock final : public Object {
    public:
        jxx::Ptr<String> toString() const override {
            return JXX_NEW<String>("ClassLoadingLock");
        }
    };

    // Enumeration implementation for getResources()
    class VectorUrlEnumeration final
        : public Object
        , public jxx::util::Enumeration<jxx::Ptr<jxx::net::URL>>
    {
    public:
        explicit VectorUrlEnumeration(std::vector<jxx::Ptr<jxx::net::URL>> items);

        jbool hasMoreElements() override;
        jxx::Ptr<jxx::net::URL> nextElement() override;

    private:
        std::vector<jxx::Ptr<jxx::net::URL>> items_;
        std::size_t idx_ = 0;
    };
};

} // namespace jxx::lang