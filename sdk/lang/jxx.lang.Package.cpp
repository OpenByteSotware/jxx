#include "lang/jxx.lang.Package.h"

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

#include <cstdint>
#include <vector>

namespace jxx::lang {

std::mutex Package::registryMutex_;
std::unordered_map<std::string, std::weak_ptr<Package>>
    Package::registry_;

jxx::Ptr<Package> Package::definePackage(
    const jxx::Ptr<String>& name) {
    if (name == nullptr) {
        throw NullPointerException("name");
    }
    const auto key = name->utf8();
    std::lock_guard<std::mutex> lock(registryMutex_);
    const auto found = registry_.find(key);
    if (found != registry_.end()) {
        if (auto value = found->second.lock()) {
            return value;
        }
    }
    auto value = jxx::NEW<Package>(name);
    registry_[key] = value;
    return value;
}

jxx::Ptr<Package> Package::getPackage(
    const jxx::Ptr<String>& name) {
    if (name == nullptr) {
        throw NullPointerException("name");
    }
    std::lock_guard<std::mutex> lock(registryMutex_);
    const auto found = registry_.find(name->utf8());
    return found == registry_.end()
        ? nullptr
        : found->second.lock();
}

jxx::Ptr<JxxArray<jxx::Ptr<Package>, 1>>
Package::getPackages() {
    std::vector<jxx::Ptr<Package>> values;
    {
        std::lock_guard<std::mutex> lock(registryMutex_);
        for (auto iterator = registry_.begin();
             iterator != registry_.end();) {
            if (auto value = iterator->second.lock()) {
                values.push_back(value);
                ++iterator;
            }
            else {
                iterator = registry_.erase(iterator);
            }
        }
    }
    auto result = jxx::NEW<JxxArray<jxx::Ptr<Package>, 1>>(
        static_cast<std::uint32_t>(values.size()));
    for (std::uint32_t index = 0;
         index < static_cast<std::uint32_t>(values.size());
         ++index) {
        (*result)[static_cast<jint>(index)] = values[index];
    }
    return result;
}

Package::Package(const jxx::Ptr<String>& name)
    : name_(name) {
    if (name_ == nullptr) {
        throw NullPointerException("name");
    }
}

jxx::Ptr<String> Package::getName() const {
    return name_;
}

jxx::Ptr<String> Package::toString() const {
    return jxx::NEW<String>(
        std::string("package ") + name_->utf8());
}

} // namespace jxx::lang
