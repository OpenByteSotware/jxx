
#include "io/jxx.io.ByteArrayInputStream.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.ClassNotFoundException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "lang/jxx.lang.ClassLoader.h"

namespace jxx::lang {
    std::mutex ClassLoader::systemMutex_{};
    jxx::Ptr<ClassLoader> ClassLoader::systemLoader_{};

    ClassLoader::VectorUrlEnumeration::VectorUrlEnumeration(std::vector<jxx::Ptr<jxx::net::URL>> items)
        : items_(std::move(items)) {}

    jbool ClassLoader::VectorUrlEnumeration::hasMoreElements() {
        return idx_ < items_.size();
    }

    jxx::Ptr<jxx::net::URL> ClassLoader::VectorUrlEnumeration::nextElement() {
        if (idx_ >= items_.size()) {
            throw jxx::util::NoSuchElementException(jxx::NEW<String>("nextElement"));
        }
        return items_[idx_++];
    }

    ClassLoader::ClassLoader()
        : parent_(nullptr) {
        parent_ = getSystemClassLoader();
    }

    ClassLoader::ClassLoader(const jxx::Ptr<ClassLoader>& parent)
        : parent_(parent) {}

    jxx::Ptr<ClassLoader> ClassLoader::getParent() const { return parent_; }

    jxx::Ptr<ClassLoader> ClassLoader::getSystemClassLoader() {
        std::lock_guard<std::mutex> lk(systemMutex_);
        if (systemLoader_ != nullptr) {
            return systemLoader_;
        }
        systemLoader_ =
            jxx::NEW<ClassLoader>(jxx::Ptr<ClassLoader>(nullptr));
        return systemLoader_;
    }

    jxx::Ptr<jxx::net::URL> ClassLoader::getSystemResource(const jxx::Ptr<String>& name) {
        return getSystemClassLoader()->getResource(name);
    }
    jxx::Ptr<jxx::util::Enumeration<jxx::net::URL>> ClassLoader::getSystemResources(const jxx::Ptr<String>& name) {
        return getSystemClassLoader()->getResources(name);
    }
    jxx::Ptr<jxx::io::InputStream> ClassLoader::getSystemResourceAsStream(const jxx::Ptr<String>& name) {
        return getSystemClassLoader()->getResourceAsStream(name);
    }

    jxx::Ptr<Object> ClassLoader::getClassLoadingLock(const jxx::Ptr<String>& className) {
        if (!className) throw NullPointerException(jxx::NEW<String>("className"));
        const std::string n = className->utf8();

        std::lock_guard<std::mutex> lk(lockMapMutex_);
        auto it = loadLocks_.find(n);
        if (it != loadLocks_.end()) {
            if (auto existing = it->second.lock()) return existing;
        }
        auto lockObj = jxx::NEW<LoadingLock>();
        loadLocks_[n] = lockObj;
        return lockObj;
    }

    jxx::Ptr<ClassAny> ClassLoader::loadClass(const jxx::Ptr<String>& name) {
        return loadClass(name, false);
    }

    jxx::Ptr<String> ClassLoader::findLibrary(
        const jxx::Ptr<String>& /*libraryName*/) {
        return nullptr;
    }

    jbool ClassLoader::registerAsParallelCapable() {
        return true;
    }

    jxx::Ptr<ClassAny> ClassLoader::loadClass(
        const jxx::Ptr<String>& name,
        jbool resolve) {
        if (name == nullptr) {
            throw NullPointerException(jxx::NEW<String>("name"));
        }

        const std::string binaryName = name->utf8();
        const auto loadingLock = getClassLoadingLock(name);
        return loadingLock->synchronized([&]() -> jxx::Ptr<ClassAny> {
            auto loaded = findLoadedClass(name);
            if (loaded == nullptr) {
                if (parent_ != nullptr) {
                    try {
                        loaded = parent_->loadClass(name, false);
                    }
                    catch (const ClassNotFoundException&) {
                        loaded = nullptr;
                    }
                }
                else {
                    try {
                        loaded = findSystemClass(name);
                    }
                    catch (const ClassNotFoundException&) {
                        loaded = nullptr;
                    }
                }

                if (loaded == nullptr) {
                    loaded = findClass(name);
                    if (loaded == nullptr) {
                        throw ClassNotFoundException(name);
                    }
                    loaded->meta_.classLoader =
                        ::jxx::CAST<ClassLoader>(thisPtr());
                }

                std::lock_guard<std::mutex> cacheLock(loadedMutex_);
                loadedByName_[binaryName] = loaded;
            }

            if (resolve) {
                resolveClass(loaded);
            }
            return loaded;
        });
    }

    jxx::Ptr<ClassAny> ClassLoader::findLoadedClass(const jxx::Ptr<String>& name) {
        if (!name) throw NullPointerException(jxx::NEW<String>("name"));
        const std::string n = name->utf8();
        std::lock_guard<std::mutex> lk(loadedMutex_);
        const auto it = loadedByName_.find(n);
        return it == loadedByName_.end()
            ? nullptr
            : it->second;
    }

    jxx::Ptr<ClassAny> ClassLoader::findSystemClass(const jxx::Ptr<String>& name) {
        return ClassAny::forName(name);
    }

    jxx::Ptr<ClassAny> ClassLoader::findClass(const jxx::Ptr<String>& name) {
        throw ClassNotFoundException(name);
    }

    jxx::Ptr<ClassAny> ClassLoader::defineClass(
        const jxx::Ptr<String>& name,
        const jxx::lang::ByteArray& bytes,
        jint offset,
        jint length) {

        if (bytes == nullptr) {
            throw NullPointerException(
                jxx::NEW<String>("bytes"));
        }

        if (offset < 0 ||
            length < 0 ||
            offset > bytes->length - length) {

            throw IndexOutOfBoundsException(
                jxx::NEW<String>(
                    "Invalid class byte range"));
        }

        const std::string className =
            name == nullptr
            ? std::string("<unnamed>")
            : name->utf8();

        throw UnsupportedOperationException(
            jxx::NEW<String>(
                std::string(
                    "ClassLoader.defineClass is not supported for ") +
                className));
    }

    jxx::Ptr<ClassAny> ClassLoader::defineClass(
        const jxx::lang::ByteArray& bytes,
        jint offset,
        jint length) {

        const jxx::Ptr<String>& unnamedClass =
            nullptr;

        return defineClass(
            unnamedClass,
            bytes,
            offset,
            length);
    }

    void ClassLoader::resolveClass(const jxx::Ptr<ClassAny>&) {
        // no-op in JXX
    }

    void ClassLoader::addResource(
        const jxx::Ptr<String>& name,
        const jxx::lang::ByteArray& bytes) {
        if (name == nullptr) {
            throw NullPointerException("name");
        }
        if (bytes == nullptr) {
            throw NullPointerException("bytes");
        }

        auto stored = jxx::NEW<jxx::lang::ByteArrayType>(
            static_cast<std::uint32_t>(bytes->length));
        for (std::uint32_t index = 0;
             index < static_cast<std::uint32_t>(bytes->length);
             ++index) {
            (*stored)[static_cast<jint>(index)] =
                (*bytes)[static_cast<jint>(index)];
        }

        std::lock_guard<std::mutex> lock(resourceMutex_);
        resources_[name->utf8()] = stored;
    }

    jxx::Ptr<jxx::net::URL> ClassLoader::findResource(const jxx::Ptr<String>& name) {
        if (!name) throw NullPointerException(jxx::NEW<String>("name"));
        const std::string n = name->utf8();
        std::lock_guard<std::mutex> lk(resourceMutex_);
        if (resources_.find(n) == resources_.end()) return nullptr;
        std::string spec = "jxxres://";
        spec += n;
        return jxx::NEW<jxx::net::URL>(jxx::NEW<String>(spec.c_str()));
    }

    jxx::Ptr<jxx::util::Enumeration<jxx::net::URL>> ClassLoader::findResources(const jxx::Ptr<String>& name) {
        std::vector<jxx::Ptr<jxx::net::URL>> v;
        if (auto u = findResource(name)) v.push_back(u);
        return jxx::NEW<VectorUrlEnumeration>(std::move(v));
    }

    jxx::Ptr<jxx::net::URL> ClassLoader::getResource(const jxx::Ptr<String>& name) {
        if (!name) throw NullPointerException(jxx::NEW<String>("name"));
        if (parent_) {
            if (auto u = parent_->getResource(name)) return u;
        }
        return findResource(name);
    }

    jxx::Ptr<jxx::util::Enumeration<jxx::net::URL>> ClassLoader::getResources(const jxx::Ptr<String>& name) {
        if (!name) throw NullPointerException(jxx::NEW<String>("name"));

        std::vector<jxx::Ptr<jxx::net::URL>> all;
        if (parent_) {
            auto pe = parent_->getResources(name);
            if (pe) while (pe->hasMoreElements()) all.push_back(pe->nextElement());
        }
        auto le = findResources(name);
        if (le) while (le->hasMoreElements()) all.push_back(le->nextElement());

        return jxx::NEW<VectorUrlEnumeration>(std::move(all));
    }

    jxx::Ptr<jxx::io::InputStream> ClassLoader::getResourceAsStream(const jxx::Ptr<String>& name) {
        if (!name) throw NullPointerException(jxx::NEW<String>("name"));
        if (parent_) {
            if (auto s = parent_->getResourceAsStream(name)) return s;
        }
        const std::string n = name->utf8();
        std::lock_guard<std::mutex> lk(resourceMutex_);
        auto it = resources_.find(n);
        if (it == resources_.end()) return nullptr;
        return jxx::NEW<jxx::io::ByteArrayInputStream>(it->second);
    }


    void ClassLoader::setDefaultAssertionStatus(jbool enabled) {
        std::lock_guard<std::mutex> lock(assertionMutex_);
        defaultAssertionStatus_ = enabled;
    }

    void ClassLoader::setPackageAssertionStatus(
        const jxx::Ptr<String>& packageName,
        jbool enabled) {
        std::lock_guard<std::mutex> lock(assertionMutex_);
        packageAssertion_[
            packageName == nullptr
                ? std::string()
                : packageName->utf8()] = enabled;
    }

    void ClassLoader::setClassAssertionStatus(
        const jxx::Ptr<String>& className,
        jbool enabled) {
        if (className == nullptr) {
            throw NullPointerException("className");
        }
        std::lock_guard<std::mutex> lock(assertionMutex_);
        classAssertion_[className->utf8()] = enabled;
    }

    void ClassLoader::clearAssertionStatus() {
        std::lock_guard<std::mutex> lock(assertionMutex_);
        classAssertion_.clear();
        packageAssertion_.clear();
        defaultAssertionStatus_ = false;
    }

    jbool ClassLoader::desiredAssertionStatus(
        const jxx::Ptr<ClassAny>& clazz) const {
        if (clazz == nullptr) {
            throw NullPointerException("clazz");
        }

        const auto className = clazz->getName()->utf8();
        std::lock_guard<std::mutex> lock(assertionMutex_);

        const auto classSetting = classAssertion_.find(className);
        if (classSetting != classAssertion_.end()) {
            return classSetting->second;
        }

        auto packageEnd = className.find_last_of('.');
        while (packageEnd != std::string::npos) {
            const auto packageName =
                className.substr(0U, packageEnd);
            const auto packageSetting =
                packageAssertion_.find(packageName);
            if (packageSetting != packageAssertion_.end()) {
                return packageSetting->second;
            }
            packageEnd = packageName.find_last_of('.');
        }

        const auto unnamedSetting =
            packageAssertion_.find(std::string());
        return unnamedSetting != packageAssertion_.end()
            ? unnamedSetting->second
            : defaultAssertionStatus_;
    }

    // Packages
    jxx::Ptr<Package> ClassLoader::definePackage(
        const jxx::Ptr<String>& name) {
        return definePackage(name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    }

    ::jxx::Ptr<Package> ClassLoader::definePackage(
        const ::jxx::Ptr<String>& name,
        const ::jxx::Ptr<String>& specificationTitle,
        const ::jxx::Ptr<String>& specificationVersion,
        const ::jxx::Ptr<String>& specificationVendor,
        const ::jxx::Ptr<String>& implementationTitle,
        const ::jxx::Ptr<String>& implementationVersion,
        const ::jxx::Ptr<String>& implementationVendor,
        const ::jxx::Ptr<::jxx::net::URL>& sealBase) {
        if (name == nullptr) throw NullPointerException("name");
        const auto packageName = name->utf8();
        std::lock_guard<std::mutex> lock(pkgMutex_);
        if (packages_.find(packageName) != packages_.end()) {
            throw IllegalArgumentException("package already defined");
        }
        auto result = jxx::NEW<Package>(
            name, specificationTitle, specificationVersion, specificationVendor,
            implementationTitle, implementationVersion, implementationVendor, sealBase);
        packages_[packageName] = result;
        return result;
    }

    jxx::Ptr<Package> ClassLoader::getPackage(
        const jxx::Ptr<String>& name) {
        if (name == nullptr) {
            throw NullPointerException("name");
        }
        {
            std::lock_guard<std::mutex> lock(pkgMutex_);
            const auto existing = packages_.find(name->utf8());
            if (existing != packages_.end()) {
                return existing->second;
            }
        }
        return parent_ == nullptr
            ? nullptr
            : parent_->getPackage(name);
    }

    jxx::Ptr<JxxArray<jxx::Ptr<Package>, 1>>
    ClassLoader::getPackages() {
        std::vector<jxx::Ptr<Package>> values;
        std::unordered_set<std::string> names;

        {
            std::lock_guard<std::mutex> lock(pkgMutex_);
            values.reserve(packages_.size());
            for (const auto& entry : packages_) {
                names.insert(entry.first);
                values.push_back(entry.second);
            }
        }

        if (parent_ != nullptr) {
            const auto inherited = parent_->getPackages();
            for (std::uint32_t index = 0;
                 index < inherited->length;
                 ++index) {
                const auto packageValue =
                    (*inherited)[static_cast<jint>(index)];
                const auto packageName =
                    packageValue->getName()->utf8();
                if (names.insert(packageName).second) {
                    values.push_back(packageValue);
                }
            }
        }

        auto result =
            jxx::NEW<JxxArray<jxx::Ptr<Package>, 1>>(
                static_cast<std::uint32_t>(values.size()));
        for (std::uint32_t index = 0;
             index < static_cast<std::uint32_t>(values.size());
             ++index) {
            (*result)[static_cast<jint>(index)] = values[index];
        }
        return result;
    }

} // namespace jxx::lang


