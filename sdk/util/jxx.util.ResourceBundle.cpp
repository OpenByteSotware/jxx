#include "util/jxx.util.ResourceBundle.h"

#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.MissingResourceException.h"
#include "util/jxx.util.HashSet.h"
#include "util/jxx.util.NoSuchElementException.h"

namespace jxx::util {
namespace {

using Factory = ResourceBundle::BundleFactory;

class KeyEnumeration final : public Enumeration<jxx::lang::String> {
public:
    explicit KeyEnumeration(
        std::vector<jxx::Ptr<jxx::lang::String>> values)
        : values_(std::move(values)) {
    }

    jxx::lang::jbool hasMoreElements() override {
        return index_ < values_.size();
    }

    jxx::Ptr<jxx::lang::String> nextElement() override {
        if (!hasMoreElements()) {
            throw NoSuchElementException();
        }
        return values_[index_++];
    }

private:
    std::vector<jxx::Ptr<jxx::lang::String>> values_;
    std::size_t index_ = 0;
};

std::mutex& registryMutex() {
    static std::mutex value;
    return value;
}

std::unordered_map<std::string, Factory>& registry() {
    static std::unordered_map<std::string, Factory> value;
    return value;
}

std::unordered_map<std::string, jxx::Ptr<ResourceBundle>>& cache() {
    static std::unordered_map<std::string, jxx::Ptr<ResourceBundle>> value;
    return value;
}

void appendCandidate(
    std::vector<std::string>& candidates,
    const std::string& candidate) {
    if (!candidate.empty()) {
        candidates.push_back(candidate);
    }
}

std::vector<std::string> candidateNames(
    const std::string& baseName,
    const jxx::Ptr<Locale>& locale) {
    std::vector<std::string> result;
    const auto language = locale->getLanguage()->utf8();
    const auto script = locale->getScript()->utf8();
    const auto country = locale->getCountry()->utf8();
    const auto variant = locale->getVariant()->utf8();

    std::string suffix;
    if (!language.empty()) {
        suffix = "_" + language;
        if (!script.empty()) suffix += "_" + script;
        if (!country.empty()) suffix += "_" + country;
        if (!variant.empty()) suffix += "_" + variant;
        appendCandidate(result, baseName + suffix);

        if (!variant.empty()) {
            suffix.erase(suffix.rfind('_'));
            appendCandidate(result, baseName + suffix);
        }
        if (!country.empty()) {
            suffix.erase(suffix.rfind('_'));
            appendCandidate(result, baseName + suffix);
        }
        if (!script.empty()) {
            appendCandidate(result, baseName + "_" + language);
        }
    }
    result.push_back(baseName);
    return result;
}

} // namespace

jxx::Ptr<ResourceBundle> ResourceBundle::getBundle(
    const jxx::Ptr<jxx::lang::String>& baseName) {
    return getBundle(baseName, Locale::getDefault());
}

jxx::Ptr<ResourceBundle> ResourceBundle::getBundle(
    const jxx::Ptr<jxx::lang::String>& baseName,
    const jxx::Ptr<Locale>& locale) {
    if (baseName == nullptr || locale == nullptr) {
        throw jxx::lang::NullPointerException();
    }

    const auto base = baseName->utf8();
    const auto candidates = candidateNames(base, locale);
    const auto cacheKey = base + "|" + locale->toLanguageTag()->utf8();
    std::lock_guard<std::mutex> lock(registryMutex());

    const auto cached = cache().find(cacheKey);
    if (cached != cache().end()) {
        return cached->second;
    }

    jxx::Ptr<ResourceBundle> parent;
    jxx::Ptr<ResourceBundle> selected;
    for (auto iterator = candidates.rbegin();
         iterator != candidates.rend();
         ++iterator) {
        const auto provider = registry().find(*iterator);
        if (provider == registry().end()) {
            continue;
        }
        auto bundle = provider->second();
        if (bundle == nullptr) {
            continue;
        }
        bundle->locale_ = locale;
        bundle->baseBundleName_ = baseName;
        bundle->setParent(parent);
        parent = bundle;
        selected = bundle;
    }

    if (selected != nullptr) {
        cache()[cacheKey] = selected;
        return selected;
    }

    throw MissingResourceException(
        jxx::NEW<jxx::lang::String>("Cannot find resource bundle"),
        baseName,
        jxx::NEW<jxx::lang::String>(""));
}

void ResourceBundle::clearCache() {
    std::lock_guard<std::mutex> lock(registryMutex());
    cache().clear();
}

void ResourceBundle::registerBundle(
    const jxx::Ptr<jxx::lang::String>& bundleName,
    const BundleFactory& factory) {
    if (bundleName == nullptr || !factory) {
        throw jxx::lang::NullPointerException();
    }
    std::lock_guard<std::mutex> lock(registryMutex());
    registry()[bundleName->utf8()] = factory;
    cache().clear();
}

jxx::Ptr<jxx::lang::Object> ResourceBundle::getObject(
    const jxx::Ptr<jxx::lang::String>& key) {
    if (key == nullptr) {
        throw jxx::lang::NullPointerException();
    }

    auto value = handleGetObject(key);
    if (value != nullptr) {
        return value;
    }
    if (parent_ != nullptr) {
        return parent_->getObject(key);
    }

    throw MissingResourceException(
        jxx::NEW<jxx::lang::String>("Cannot find resource for key"),
        baseBundleName_,
        key);
}

jxx::Ptr<jxx::lang::String> ResourceBundle::getString(
    const jxx::Ptr<jxx::lang::String>& key) {
    return jxx::CAST<jxx::lang::String>(getObject(key));
}

jxx::Ptr<Enumeration<jxx::lang::String>> ResourceBundle::getKeys() {
    std::vector<jxx::Ptr<jxx::lang::String>> keys;
    std::unordered_set<std::string> seen;

    for (auto bundle = jxx::CAST<ResourceBundle>(thisPtr());
         bundle != nullptr;
         bundle = bundle->parent_) {
        auto local = bundle->getLocalKeys();
        if (local == nullptr) {
            continue;
        }
        while (local->hasMoreElements()) {
            auto key = local->nextElement();
            if (key != nullptr && seen.insert(key->utf8()).second) {
                keys.push_back(key);
            }
        }
    }

    return jxx::Ptr<Enumeration<jxx::lang::String>>(
        new KeyEnumeration(std::move(keys)));
}


jxx::lang::jbool ResourceBundle::containsKey(
    const jxx::Ptr<jxx::lang::String>& key) {
    if (key == nullptr) {
        throw jxx::lang::NullPointerException();
    }

    for (auto bundle = jxx::CAST<ResourceBundle>(thisPtr());
         bundle != nullptr;
         bundle = bundle->parent_) {
        if (bundle->handleGetObject(key) != nullptr) {
            return true;
        }
    }
    return false;
}

jxx::Ptr<Set<jxx::lang::String>> ResourceBundle::keySet() {
    auto result = jxx::NEW<HashSet<jxx::lang::String>>();
    auto keys = getKeys();
    while (keys->hasMoreElements()) {
        result->add(keys->nextElement());
    }
    return jxx::CAST<Set<jxx::lang::String>>(result);
}

void ResourceBundle::setParent(
    const jxx::Ptr<ResourceBundle>& parent) {
    parent_ = parent;
}

jxx::Ptr<ResourceBundle> ResourceBundle::getParent() const {
    return parent_;
}

jxx::Ptr<Locale> ResourceBundle::getLocale() const {
    return locale_;
}

jxx::Ptr<jxx::lang::String>
ResourceBundle::getBaseBundleName() const {
    return baseBundleName_;
}

} // namespace jxx::util
