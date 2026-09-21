#pragma once

#include <mutex>
#include <string>
#include <unordered_map>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::net { class URL; }

namespace jxx::lang {

class ClassAny;
class String;

class Package final : public ClassBase<Package, Object> {
public:
    using JxxSuper = Object;
    using Super = ClassBase<Package, JxxSuper>;
    using JxxClassInfoMarker = ClassInfo<Package, JxxSuper>;

    static jxx::Ptr<ClassAny> Class();

    static jxx::Ptr<Package> getPackage(const jxx::Ptr<String>& name);
    static jxx::Ptr<JxxArray<jxx::Ptr<Package>, 1>> getPackages();

    jxx::Ptr<String> getName() const;
    jxx::Ptr<String> getSpecificationTitle() const;
    jxx::Ptr<String> getSpecificationVersion() const;
    jxx::Ptr<String> getSpecificationVendor() const;
    jxx::Ptr<String> getImplementationTitle() const;
    jxx::Ptr<String> getImplementationVersion() const;
    jxx::Ptr<String> getImplementationVendor() const;

    jbool isSealed() const noexcept;
    jbool isSealed(const ::jxx::Ptr<::jxx::net::URL>& sealBase) const;
    jbool isCompatibleWith(const jxx::Ptr<String>& desired) const;
    jxx::Ptr<String> toString() const override;

    static jxx::Ptr<Package> definePackage(const jxx::Ptr<String>& name);
    static jxx::Ptr<Package> definePackage(
        const jxx::Ptr<String>& name,
        const jxx::Ptr<String>& specificationTitle,
        const jxx::Ptr<String>& specificationVersion,
        const jxx::Ptr<String>& specificationVendor,
        const jxx::Ptr<String>& implementationTitle,
        const jxx::Ptr<String>& implementationVersion,
        const jxx::Ptr<String>& implementationVendor,
        const ::jxx::Ptr<::jxx::net::URL>& sealBase);

    Package(
        const jxx::Ptr<String>& name,
        const jxx::Ptr<String>& specificationTitle = nullptr,
        const jxx::Ptr<String>& specificationVersion = nullptr,
        const jxx::Ptr<String>& specificationVendor = nullptr,
        const jxx::Ptr<String>& implementationTitle = nullptr,
        const jxx::Ptr<String>& implementationVersion = nullptr,
        const jxx::Ptr<String>& implementationVendor = nullptr,
        const ::jxx::Ptr<::jxx::net::URL>& sealBase = nullptr);

private:
    jxx::Ptr<String> name_;
    jxx::Ptr<String> specificationTitle_;
    jxx::Ptr<String> specificationVersion_;
    jxx::Ptr<String> specificationVendor_;
    jxx::Ptr<String> implementationTitle_;
    jxx::Ptr<String> implementationVersion_;
    jxx::Ptr<String> implementationVendor_;
    ::jxx::Ptr<::jxx::net::URL> sealBase_;

    static std::mutex registryMutex_;
    static std::unordered_map<std::string, std::weak_ptr<Package>> registry_;
};

} // namespace jxx::lang
