#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.IllegalArgumentException.h"
#include "jxx.lang.IllegalStateException.h"
#include "jxx.lang.ClassNotFoundException.h"
#include "jxx.lang.UnsupportedOperationException.h"
#include "jxx.lang.NoSuchElementException.h"
#include "io/jxx.io.ByteArrayInputStream.h"

#include "jxx.lang.ClassLoader.h"

namespace jxx::lang {

std::mutex ClassLoader::systemMutex_{};
std::weak_ptr<ClassLoader> ClassLoader::systemLoader_{};

// ---------------- VectorUrlEnumeration ----------------
ClassLoader::VectorUrlEnumeration::VectorUrlEnumeration(std::vector<jxx::Ptr<jxx::net::URL>> items)
    : items_(std::move(items)) {}

jbool ClassLoader::VectorUrlEnumeration::hasMoreElements() {
    return idx_ < items_.size();
}

jxx::Ptr<jxx::net::URL> ClassLoader::VectorUrlEnumeration::nextElement() {
    if (idx_ >= items_.size()) {
        throw jxx::lang::NoSuchElementException(JXX_NEW<jxx::lang::String>("nextElement"));
    }
    return items_[idx_++];
}

// ---------------- constructors ----------------
ClassLoader::ClassLoader()
    : parent_(nullptr) {
    // JXX: default parent is the system loader (practical)
    parent_ = getSystemClassLoader();
}

ClassLoader::ClassLoader(jxx::Ptr<ClassLoader> parent)
    : parent_(std::move(parent)) {}

// ---------------- parent/system ----------------
jxx::Ptr<ClassLoader> ClassLoader::getParent() const {
    return parent_;
}

jxx::Ptr<ClassLoader> ClassLoader::getSystemClassLoader() {
    std::lock_guard<std::mutex> lk(systemMutex_);
    if (auto s = systemLoader_.lock()) return s;

    // System loader has no parent (bootstrap is implicit in Java)
    auto sys = JXX_NEW<ClassLoader>(jxx::Ptr<ClassLoader>(nullptr));
    systemLoader_ = sys;
    return sys;
}

// ---------------- system resource helpers ----------------
jxx::Ptr<jxx::net::URL> ClassLoader::getSystemResource(jxx::Ptr<String> name) {
    return getSystemClassLoader()->getResource(name);
}

jxx::Ptr<jxx::util::Enumeration<jxx::Ptr<jxx::net::URL>>> ClassLoader::getSystemResources(jxx::Ptr<String> name) {
    return getSystemClassLoader()->getResources(name);
}

jxx::Ptr<jxx::io::InputStream> ClassLoader::getSystemResourceAsStream(jxx::Ptr<String> name) {
    return getSystemClassLoader()->getResourceAsStream(name);
}

// ---------------- loading locks ----------------
jxx::Ptr<Object> ClassLoader::getClassLoadingLock(jxx::Ptr<String> className) {
    if (!className) throw NullPointerException(JXX_NEW<String>("className"));
    const std::string n = className->utf8();

    std::lock_guard<std::mutex> lk(lockMapMutex_);
    auto it = loadLocks_.find(n);
    if (it != loadLocks_.end()) {
        if (auto existing = it->second.lock()) return existing;
    }

    auto lockObj = JXX_NEW<LoadingLock>();
    loadLocks_[n] = lockObj;
    return lockObj;
}

// ---------------- class loading ----------------
jxx::Ptr<ClassAny> ClassLoader::loadClass(jxx::Ptr<String> name) {
    return loadClass(name, false);
}

jxx::Ptr<ClassAny> ClassLoader::loadClass(jxx::Ptr<String> name, jbool resolve) {
    if (!name) throw NullPointerException(JXX_NEW<String>("name"));
    const std::string n = name->utf8();

    auto lockObj = getClassLoadingLock(name);

    return this->synchronized([&]()->jxx::Ptr<ClassAny> {
        // 1) Already loaded by this loader?
        if (auto loaded = findLoadedClass(name)) {
            if (resolve) resolveClass(loaded);
            return loaded;
        }

        // 2) Parent-first delegation
        if (parent_) {
            try {
                auto c = parent_->loadClass(name, false);
                if (c) {
                    std::lock_guard<std::mutex> lk2(loadedMutex_);
                    loadedByName_[n] = c;
                    if (resolve) resolveClass(c);
                    return c;
                }
            } catch (const ClassNotFoundException&) {
                // fall through
            }
        }

        // 3) Bootstrap/system lookup (JXX: global ClassAny registry)
        try {
            auto c = findSystemClass(name);
            if (c) {
                std::lock_guard<std::mutex> lk2(loadedMutex_);
                loadedByName_[n] = c;
                if (resolve) resolveClass(c);
                return c;
            }
        } catch (const ClassNotFoundException&) {
            // fall through
        }

        // 4) Loader-specific lookup (override point)
        auto c = findClass(name);
        if (!c) throw ClassNotFoundException(name);

        {
            std::lock_guard<std::mutex> lk2(loadedMutex_);
            loadedByName_[n] = c;
        }
        if (resolve) resolveClass(c);
        return c;
    });
}

jxx::Ptr<ClassAny> ClassLoader::findLoadedClass(jxx::Ptr<String> name) {
    if (!name) throw NullPointerException(JXX_NEW<String>("name"));
    const std::string n = name->utf8();

    std::lock_guard<std::mutex> lk(loadedMutex_);
    auto it = loadedByName_.find(n);
    if (it == loadedByName_.end()) return nullptr;
    return it->second.lock();
}

jxx::Ptr<ClassAny> ClassLoader::findSystemClass(jxx::Ptr<String> name) {
    return ClassAny::forName(name);
}

jxx::Ptr<ClassAny> ClassLoader::findClass(jxx::Ptr<String> name) {
    // Base loader cannot load new classes by itself.
    throw ClassNotFoundException(name);
}

void ClassLoader::resolveClass(jxx::Ptr<ClassAny> /*c*/) {
    // JVM links here; JXX registry implies linkage already.
}

// ---------------- defineClass (bytecode) ----------------
jxx::Ptr<ClassAny> ClassLoader::defineClass(jxx::Ptr<String> /*name*/, jxx::Ptr<ByteArray> /*b*/, jint /*off*/, jint /*len*/) {
    throw UnsupportedOperationException(JXX_NEW<String>("defineClass(bytecode) not supported"));
}

jxx::Ptr<ClassAny> ClassLoader::defineClass(jxx::Ptr<ByteArray> /*b*/, jint /*off*/, jint /*len*/) {
    throw UnsupportedOperationException(JXX_NEW<String>("defineClass(bytecode) not supported"));
}

// ---------------- resources ----------------
void ClassLoader::addResource(jxx::Ptr<String> name, jxx::Ptr<ByteArray> bytes) {
    if (!name) throw NullPointerException(JXX_NEW<String>("name"));
    const std::string n = name->utf8();

    std::lock_guard<std::mutex> lk(resourceMutex_);
    resources_[n] = bytes;
}

jxx::Ptr<jxx::net::URL> ClassLoader::findResource(jxx::Ptr<String> name) {
    if (!name) throw NullPointerException(JXX_NEW<String>("name"));
    const std::string n = name->utf8();

    std::lock_guard<std::mutex> lk(resourceMutex_);
    auto it = resources_.find(n);
    if (it == resources_.end()) return nullptr;

    std::string spec = "jxxres://";
    spec += n;
    return JXX_NEW<jxx::net::URL>(JXX_NEW<String>(spec.c_str()));
}

jxx::Ptr<jxx::util::Enumeration<jxx::Ptr<jxx::net::URL>>> ClassLoader::findResources(jxx::Ptr<String> name) {
    std::vector<jxx::Ptr<jxx::net::URL>> v;
    if (auto u = findResource(name)) v.push_back(u);
    return JXX_NEW<VectorUrlEnumeration>(std::move(v));
}

jxx::Ptr<jxx::net::URL> ClassLoader::getResource(jxx::Ptr<String> name) {
    if (!name) throw NullPointerException(JXX_NEW<String>("name"));

    if (parent_) {
        if (auto u = parent_->getResource(name)) return u;
    }
    return findResource(name);
}

jxx::Ptr<jxx::util::Enumeration<jxx::Ptr<jxx::net::URL>>> ClassLoader::getResources(jxx::Ptr<String> name) {
    if (!name) throw NullPointerException(JXX_NEW<String>("name"));

    std::vector<jxx::Ptr<jxx::net::URL>> all;

    // Parent resources
    if (parent_) {
        auto pe = parent_->getResources(name);
        if (pe) {
            while (pe->hasMoreElements()) {
                all.push_back(pe->nextElement());
            }
        }
    }

    // Local resources
    auto le = findResources(name);
    if (le) {
        while (le->hasMoreElements()) {
            all.push_back(le->nextElement());
        }
    }

    return JXX_NEW<VectorUrlEnumeration>(std::move(all));
}

jxx::Ptr<jxx::io::InputStream> ClassLoader::getResourceAsStream(jxx::Ptr<String> name) {
    if (!name) throw NullPointerException(JXX_NEW<String>("name"));
    // parent-first
    if (parent_) {
        if (auto s = parent_->getResourceAsStream(name)) return s;
    }
    std::lock_guard<std::mutex> lk(resourceMutex_);
    auto it = resources_.find(name->utf8());
    if (it == resources_.end()) return nullptr;
    return JXX_NEW<jxx::io::ByteArrayInputStream>(it->second);
}

// ---------------- assertions ----------------
void ClassLoader::setDefaultAssertionStatus(jbool enabled) {
    std::lock_guard<std::mutex> lk(assertionMutex_);
    defaultAssertionStatus_ = enabled;
}

void ClassLoader::setPackageAssertionStatus(jxx::Ptr<String> packageName, jbool enabled) {
    if (!packageName) throw NullPointerException(JXX_NEW<String>("packageName"));
    std::lock_guard<std::mutex> lk(assertionMutex_);
    packageAssertion_[packageName->utf8()] = enabled;
}

void ClassLoader::setClassAssertionStatus(jxx::Ptr<String> className, jbool enabled) {
    if (!className) throw NullPointerException(JXX_NEW<String>("className"));
    std::lock_guard<std::mutex> lk(assertionMutex_);
    classAssertion_[className->utf8()] = enabled;
}

void ClassLoader::clearAssertionStatus() {
    std::lock_guard<std::mutex> lk(assertionMutex_);
    defaultAssertionStatus_ = false;
    classAssertion_.clear();
    packageAssertion_.clear();
}

jbool ClassLoader::desiredAssertionStatus(jxx::Ptr<ClassAny> clazz) const {
    if (!clazz) throw NullPointerException(JXX_NEW<String>("clazz"));
    const std::string cn = clazz->getName()->utf8();

    std::lock_guard<std::mutex> lk(assertionMutex_);

    // class-specific
    if (auto it = classAssertion_.find(cn); it != classAssertion_.end()) return it->second;

    // package-specific: longest matching prefix
    jbool found = false;
    jbool val = defaultAssertionStatus_;
    std::size_t bestLen = 0;

    for (const auto& kv : packageAssertion_) {
        const std::string& pkg = kv.first;
        if (cn.rfind(pkg + ".", 0) == 0) {
            if (pkg.size() > bestLen) {
                bestLen = pkg.size();
                val = kv.second;
                found = true;
            }
        }
    }

    return found ? val : defaultAssertionStatus_;
}

// ---------------- packages ----------------
jxx::Ptr<Package> ClassLoader::definePackage(jxx::Ptr<String> name) {
    if (!name) throw NullPointerException(JXX_NEW<String>("name"));
    const std::string n = name->utf8();

    std::lock_guard<std::mutex> lk(pkgMutex_);
    if (auto it = packages_.find(n); it != packages_.end()) {
        if (auto p = it->second.lock()) return p;
    }
    auto p = JXX_NEW<Package>(name);
    packages_[n] = p;
    return p;
}

jxx::Ptr<Package> ClassLoader::getPackage(jxx::Ptr<String> name) {
    if (!name) throw NullPointerException(JXX_NEW<String>("name"));
    const std::string n = name->utf8();

    std::lock_guard<std::mutex> lk(pkgMutex_);
    auto it = packages_.find(n);
    if (it == packages_.end()) return nullptr;
    return it->second.lock();
}

jxx::Ptr<JxxArray<jxx::Ptr<Package>, 1>> ClassLoader::getPackages() {
    std::lock_guard<std::mutex> lk(pkgMutex_);
    std::vector<jxx::Ptr<Package>> alive;
    alive.reserve(packages_.size());
    for (auto& kv : packages_) {
        if (auto p = kv.second.lock()) alive.push_back(p);
    }
    auto arr = JXX_NEW<JxxArray<jxx::Ptr<Package>, 1>>((std::uint32_t)alive.size());
    for (std::uint32_t i = 0; i < (std::uint32_t)alive.size(); ++i) (*arr)[(jint)i] = alive[i];
    return arr;
}

// ---------------- parallel capable (Java 8) ----------------
jbool ClassLoader::registerAsParallelCapable() {
    // JXX already uses per-name lock objects, so we can always succeed.
    return true;
}

// ---------------- findLibrary (Java 8 protected) ----------------
jxx::Ptr<String> ClassLoader::findLibrary(jxx::Ptr<String> /*libname*/) {
    // JVM uses this to locate native libs; JXX doesn't load JNI libs via ClassLoader.
    return nullptr;
}

} // namespace jxx::lang