#include "util/jxx.util.ResourceBundle.h"

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.MissingResourceException.h"

namespace jxx::util {
namespace {

using Factory = ResourceBundle::BundleFactory;

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
        if (!script.empty()) {
            suffix += "_" + script;
        }
        if (!country.empty()) {
            suffix += "_" + country;
        }
        if (!variant.empty()) {
            suffix += "_" + variant;
        }
        appendCandidate(result, baseName + suffix);

        if (!variant.empty()) {
            const auto position = suffix.rfind('_');
            suffix.erase(position);
            appendCandidate(result, baseName + suffix);
        }
        if (!country.empty()) {
            const auto position = suffix.rfind('_');
            suffix.erase(position);
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
    std::lock_guard<std::mutex> lock(registryMutex());

    for (const auto& candidate : candidates) {
        const auto cached = cache().find(candidate);
        if (cached != cache().end()) {
            return cached->second;
        }

        const auto provider = registry().find(candidate);
        if (provider != registry().end()) {
            auto bundle = provider->second();
            if (bundle != nullptr) {
                bundle->locale_ = locale;
                bundle->baseBundleName_ = baseName;
                cache()[candidate] = bundle;
                return bundle;
            }
        }
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
    const auto name = bundleName->utf8();
    registry()[name] = factory;
    cache().erase(name);
}

jxx::Ptr<Locale> ResourceBundle::getLocale() const {
    return locale_;
}

jxx::Ptr<jxx::lang::String>
ResourceBundle::getBaseBundleName() const {
    return baseBundleName_;
}

} // namespace jxx::util
