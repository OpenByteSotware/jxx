
#include "io/jxx.io.ByteArrayInputStream.h"
#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.ClassNotFoundException.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "jxx.lang.ClassLoader.h"

namespace jxx::lang {
    std::weak_ptr<ClassLoader> ClassLoader::systemLoader_{};

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

    ClassLoader::ClassLoader(jxx::Ptr<ClassLoader> parent)
        : parent_(std::move(parent)) {}

    jxx::Ptr<ClassLoader> ClassLoader::getParent() const { return parent_; }

    jxx::Ptr<ClassLoader> ClassLoader::getSystemClassLoader() {
        std::lock_guard<std::mutex> lk(systemMutex_);
        if (auto s = systemLoader_.lock()) return s;
        auto sys = jxx::NEW<ClassLoader>(jxx::Ptr<ClassLoader>(nullptr));
        systemLoader_ = sys;
        return sys;
    }

    jxx::Ptr<jxx::net::URL> ClassLoader::getSystemResource(jxx::Ptr<String> name) {
        return getSystemClassLoader()->getResource(name);
    }
    jxx::Ptr<jxx::util::Enumeration<jxx::Ptr<jxx::net::URL>>> ClassLoader::getSystemResources(jxx::Ptr<String> name) {
        return getSystemClassLoader()->getResources(name);
    }
    jxx::Ptr<jxx::io::InputStream> ClassLoader::getSystemResourceAsStream(jxx::Ptr<String> name) {
        return getSystemClassLoader()->getResourceAsStream(name);
    }

    jxx::Ptr<Object> ClassLoader::getClassLoadingLock(jxx::Ptr<String> className) {
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

    jxx::Ptr<ClassAny> ClassLoader::loadClass(jxx::Ptr<String> name) {
        return loadClass(name, false);
    }

    jxx::Ptr<ClassAny> ClassLoader::loadClass(jxx::Ptr<String> name, jbool resolve) {
        if (!name) throw NullPointerException(jxx::NEW<String>("name"));
        const std::string n = name->utf8();

        auto lockObj = getClassLoadingLock(name);
        return this->synchronized([&]()->jxx::Ptr<ClassAny> {
            if (auto loaded = findLoadedClass(name)) {
                if (resolve) resolveClass(loaded);
                return loaded;
            }

            if (parent_) {
                try {
                    auto c = parent_->loadClass(name, false);
                    if (c) {
                        std::lock_guard<std::mutex> lk2(loadedMutex_);
                        loadedByName_[n] = c;
                        if (resolve) resolveClass(c);
                        return c;
                    }
                }
                catch (const ClassNotFoundException&) {
                }
            }

            try {
                auto c = findSystemClass(name);
                if (c) {
                    std::lock_guard<std::mutex> lk2(loadedMutex_);
                    loadedByName_[n] = c;
                    if (resolve) resolveClass(c);
                    return c;
                }
            }
            catch (const ClassNotFoundException&) {
            }

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
        if (!name) throw NullPointerException(jxx::NEW<String>("name"));
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
        throw ClassNotFoundException(name);
    }

    void ClassLoader::resolveClass(jxx::Ptr<ClassAny>) {
        // no-op in JXX
    }

    void ClassLoader::addResource(jxx::Ptr<String> name, jxx::Ptr<jxx::lang::ByteArray> bytes) {
        if (!name) throw NullPointerException(jxx::NEW<String>("name"));
        const std::string n = name->utf8();
        std::lock_guard<std::mutex> lk(resourceMutex_);
        // TODO resources_[n] = bytes;
    }

    jxx::Ptr<jxx::net::URL> ClassLoader::findResource(jxx::Ptr<String> name) {
        if (!name) throw NullPointerException(jxx::NEW<String>("name"));
        const std::string n = name->utf8();
        std::lock_guard<std::mutex> lk(resourceMutex_);
        if (resources_.find(n) == resources_.end()) return nullptr;
        std::string spec = "jxxres://";
        spec += n;
        return jxx::NEW<jxx::net::URL>(jxx::NEW<String>(spec.c_str()));
    }

    jxx::Ptr<jxx::util::Enumeration<jxx::Ptr<jxx::net::URL>>> ClassLoader::findResources(jxx::Ptr<String> name) {
        std::vector<jxx::Ptr<jxx::net::URL>> v;
        if (auto u = findResource(name)) v.push_back(u);
        return jxx::NEW<VectorUrlEnumeration>(std::move(v));
    }

    jxx::Ptr<jxx::net::URL> ClassLoader::getResource(jxx::Ptr<String> name) {
        if (!name) throw NullPointerException(jxx::NEW<String>("name"));
        if (parent_) {
            if (auto u = parent_->getResource(name)) return u;
        }
        return findResource(name);
    }

    jxx::Ptr<jxx::util::Enumeration<jxx::Ptr<jxx::net::URL>>> ClassLoader::getResources(jxx::Ptr<String> name) {
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

    jxx::Ptr<jxx::io::InputStream> ClassLoader::getResourceAsStream(jxx::Ptr<String> name) {
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

    // Packages
    jxx::Ptr<Package> ClassLoader::definePackage(jxx::Ptr<String> name) {
        if (!name) throw NullPointerException(jxx::NEW<String>("name"));
        const std::string n = name->utf8();

        std::lock_guard<std::mutex> lk(pkgMutex_);
        if (auto it = packages_.find(n); it != packages_.end()) {
            if (auto p = it->second.lock()) return p;
        }
        auto p = jxx::NEW<Package>(name);
        packages_[n] = p;
        return p;
    }

    jxx::Ptr<Package> ClassLoader::getPackage(jxx::Ptr<String> name) {
        if (!name) throw NullPointerException(jxx::NEW<String>("name"));
        const std::string n = name->utf8();
        std::lock_guard<std::mutex> lk(pkgMutex_);
        auto it = packages_.find(n);
        if (it == packages_.end()) return nullptr;
        return it->second.lock();
    }

    jxx::Ptr<JxxArray<jxx::Ptr<Package>, 1>> ClassLoader::getPackages() {
        std::lock_guard<std::mutex> lk(pkgMutex_);
        std::vector<jxx::Ptr<Package>> alive;
        for (auto& kv : packages_) if (auto p = kv.second.lock()) alive.push_back(p);
        auto arr = jxx::NEW<JxxArray<jxx::Ptr<Package>, 1>>((std::uint32_t)alive.size());
        for (std::uint32_t i = 0; i < (std::uint32_t)alive.size(); ++i) (*arr)[(jint)i] = alive[i];
        return arr;
    }

} // namespace jxx::lang


