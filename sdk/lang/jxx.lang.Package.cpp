#include "lang/jxx.lang.Package.h"

#include <cstdint>
#include <vector>

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NumberFormatException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.URL.h"

namespace jxx::lang {
namespace {
std::vector<jlong> parseVersion(const jxx::Ptr<String>& version) {
    if (version == nullptr) throw NullPointerException();
    const auto text = version->utf8();
    if (text.empty()) throw NumberFormatException();
    std::vector<jlong> values;
    std::size_t begin = 0;
    while (begin <= text.size()) {
        const auto end = text.find('.', begin);
        const auto part = text.substr(begin, end == std::string::npos ? std::string::npos : end - begin);
        if (part.empty()) throw NumberFormatException();
        jlong value = 0;
        for (const char character : part) {
            if (character < '0' || character > '9') throw NumberFormatException();
            const auto digit = static_cast<jlong>(character - '0');
            if (value > (INT64_MAX - digit) / 10) throw NumberFormatException();
            value = value * 10 + digit;
        }
        values.push_back(value);
        if (end == std::string::npos) break;
        begin = end + 1;
    }
    return values;
}
}

std::mutex Package::registryMutex_;
std::unordered_map<std::string, std::weak_ptr<Package>> Package::registry_;

jxx::Ptr<ClassAny> Package::Class() { return JxxClassInfoMarker::Class(); }

jxx::Ptr<Package> Package::definePackage(const jxx::Ptr<String>& name) {
    return definePackage(name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

jxx::Ptr<Package> Package::definePackage(
    const jxx::Ptr<String>& name,
    const jxx::Ptr<String>& specificationTitle,
    const jxx::Ptr<String>& specificationVersion,
    const jxx::Ptr<String>& specificationVendor,
    const jxx::Ptr<String>& implementationTitle,
    const jxx::Ptr<String>& implementationVersion,
    const jxx::Ptr<String>& implementationVendor,
    const ::jxx::Ptr<::jxx::net::URL>& sealBase) {
    if (name == nullptr) throw NullPointerException();
    const auto key = name->utf8();
    std::lock_guard<std::mutex> lock(registryMutex_);
    const auto found = registry_.find(key);
    if (found != registry_.end()) {
        if (auto value = found->second.lock()) return value;
    }
    auto value = jxx::NEW<Package>(name, specificationTitle, specificationVersion,
        specificationVendor, implementationTitle, implementationVersion,
        implementationVendor, sealBase);
    registry_[key] = value;
    return value;
}

jxx::Ptr<Package> Package::getPackage(const jxx::Ptr<String>& name) {
    if (name == nullptr) throw NullPointerException();
    std::lock_guard<std::mutex> lock(registryMutex_);
    const auto found = registry_.find(name->utf8());
    return found == registry_.end() ? nullptr : found->second.lock();
}

jxx::Ptr<JxxArray<jxx::Ptr<Package>, 1>> Package::getPackages() {
    std::vector<jxx::Ptr<Package>> values;
    std::lock_guard<std::mutex> lock(registryMutex_);
    for (auto iterator = registry_.begin(); iterator != registry_.end();) {
        if (auto value = iterator->second.lock()) { values.push_back(value); ++iterator; }
        else iterator = registry_.erase(iterator);
    }
    auto result = jxx::NEW<JxxArray<jxx::Ptr<Package>, 1>>(values.size());
    for (std::uint32_t index = 0; index < result->length; ++index) (*result)[index] = values[index];
    return result;
}

Package::Package(const jxx::Ptr<String>& name,
    const jxx::Ptr<String>& specificationTitle,
    const jxx::Ptr<String>& specificationVersion,
    const jxx::Ptr<String>& specificationVendor,
    const jxx::Ptr<String>& implementationTitle,
    const jxx::Ptr<String>& implementationVersion,
    const jxx::Ptr<String>& implementationVendor,
    const ::jxx::Ptr<::jxx::net::URL>& sealBase)
    : Super(), name_(name), specificationTitle_(specificationTitle),
      specificationVersion_(specificationVersion), specificationVendor_(specificationVendor),
      implementationTitle_(implementationTitle), implementationVersion_(implementationVersion),
      implementationVendor_(implementationVendor), sealBase_(sealBase) {
    if (name_ == nullptr) throw NullPointerException();
}

jxx::Ptr<String> Package::getName() const { return name_; }
jxx::Ptr<String> Package::getSpecificationTitle() const { return specificationTitle_; }
jxx::Ptr<String> Package::getSpecificationVersion() const { return specificationVersion_; }
jxx::Ptr<String> Package::getSpecificationVendor() const { return specificationVendor_; }
jxx::Ptr<String> Package::getImplementationTitle() const { return implementationTitle_; }
jxx::Ptr<String> Package::getImplementationVersion() const { return implementationVersion_; }
jxx::Ptr<String> Package::getImplementationVendor() const { return implementationVendor_; }
jbool Package::isSealed() const noexcept { return sealBase_ != nullptr; }
jbool Package::isSealed(const ::jxx::Ptr<::jxx::net::URL>& sealBase) const {
    if (sealBase == nullptr) return false;
    return sealBase_ != nullptr && sealBase_->equals(sealBase);
}

jbool Package::isCompatibleWith(const jxx::Ptr<String>& desired) const {
    if (specificationVersion_ == nullptr) throw NumberFormatException();
    const auto available = parseVersion(specificationVersion_);
    const auto required = parseVersion(desired);
    const auto count = std::max(available.size(), required.size());
    for (std::size_t index = 0; index < count; ++index) {
        const auto left = index < available.size() ? available[index] : 0;
        const auto right = index < required.size() ? required[index] : 0;
        if (left != right) return left > right;
    }
    return true;
}

jxx::Ptr<String> Package::toString() const {
    std::string value = "package " + name_->utf8();
    if (specificationTitle_ != nullptr) value += ", " + specificationTitle_->utf8();
    if (specificationVersion_ != nullptr) value += ", version " + specificationVersion_->utf8();
    return jxx::NEW<String>(value);
}

} // namespace jxx::lang
