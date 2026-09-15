#pragma once

#include "lang/jxx.lang.ClassInfo.h"

#include <mutex>
#include <string>
#include <unordered_map>

namespace jxx::lang {

class String;

class Package final : public ClassBase<Package, Object> {
public:
    using JxxSuper = Object;
    using Super = ClassBase<Package, JxxSuper>;

    explicit Package(const jxx::Ptr<String>& name);

    static jxx::Ptr<Package> getPackage(
        const jxx::Ptr<String>& name);
    static jxx::Ptr<JxxArray<jxx::Ptr<Package>, 1>> getPackages();

    jxx::Ptr<String> getName() const;
    jxx::Ptr<String> toString() const override;

public:
    static jxx::Ptr<Package> definePackage(
        const jxx::Ptr<String>& name);

private:
    jxx::Ptr<String> name_;

    static std::mutex registryMutex_;
    static std::unordered_map<
        std::string,
        std::weak_ptr<Package>> registry_;
};

} // namespace jxx::lang
