#include "util/jxx.util.LocaleCache.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.StringPool.h"

#include <mutex>
#include <string>
#include <unordered_map>

namespace jxx::util {
namespace {
    struct LocaleKey {
        std::string language;
        std::string script;
        std::string country;
        std::string variant;

        bool operator==(const LocaleKey& other) const {
            return language == other.language && script == other.script && country == other.country && variant == other.variant;
        }
    };

    struct LocaleKeyHash {
        std::size_t operator()(const LocaleKey& key) const {
            std::hash<std::string> h;
            std::size_t result = h(key.language);
            result = result * 1315423911u + h(key.script);
            result = result * 1315423911u + h(key.country);
            result = result * 1315423911u + h(key.variant);
            return result;
        }
    };

    LocaleKey makeKey(const jxx::Ptr<jxx::lang::String>& language,
                      const jxx::Ptr<jxx::lang::String>& script,
                      const jxx::Ptr<jxx::lang::String>& country,
                      const jxx::Ptr<jxx::lang::String>& variant) {
        LocaleKey key;
        key.language = language ? language->utf8() : "";
        key.script = script ? script->utf8() : "";
        key.country = country ? country->utf8() : "";
        key.variant = variant ? variant->utf8() : "";
        return key;
    }

    std::unordered_map<LocaleKey, jxx::Ptr<Locale>, LocaleKeyHash>& cache() {
        static std::unordered_map<LocaleKey, jxx::Ptr<Locale>, LocaleKeyHash> c;
        return c;
    }

    std::mutex& cacheMutex() {
        static std::mutex m;
        return m;
    }
}

jxx::Ptr<Locale> LocaleCache::get(const jxx::Ptr<jxx::lang::String>& language,
                                  const jxx::Ptr<jxx::lang::String>& script,
                                  const jxx::Ptr<jxx::lang::String>& country,
                                  const jxx::Ptr<jxx::lang::String>& variant) {
    auto normLanguage = language ? language->toLowerCase() : StringPool::intern("");
    auto normScript = script ? script : StringPool::intern("");
    auto normCountry = country ? country->toUpperCase() : StringPool::intern("");
    auto normVariant = variant ? variant : StringPool::intern("");

    LocaleKey key = makeKey(normLanguage, normScript, normCountry, normVariant);

    std::lock_guard<std::mutex> lock(cacheMutex());
    auto& c = cache();
    auto it = c.find(key);
    if (it != c.end()) {
        return it->second;
    }

    auto locale = jxx::NEW<Locale>(normLanguage, normScript, normCountry, normVariant);
    c.emplace(key, locale);
    return locale;
}

} // namespace jxx::util
