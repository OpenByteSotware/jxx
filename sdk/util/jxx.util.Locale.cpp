#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "lang/jxx.lang.String.h"
#include "jxx.util.LocaleCache.h"
#include "jxx.util.LocaleData.h"
#include "jxx.util.StringPool.h"
#include "jxx.util.Locale.h"


namespace jxx::util {
namespace {
    std::string cstr(const jxx::Ptr<jxx::lang::String>& s) {
        return s ? s->utf8() : std::string();
    }

    jxx::Ptr<jxx::lang::String> emptyString() {
        return StringPool::intern("");
    }

    jxx::Ptr<jxx::lang::String> join(const std::vector<std::string>& parts, const char* sep) {
        std::ostringstream os;
        bool first = true;
        for (const auto& part : parts) {
            if (part.empty()) continue;
            if (!first) os << sep;
            os << part;
            first = false;
        }
        return jxx::lang::String::valueOf(os.str().c_str());
    }

    std::vector<std::string> split(const std::string& s, char sep) {
        std::vector<std::string> out;
        std::string token;
        for (char ch : s) {
            if (ch == sep) {
                out.push_back(token);
                token.clear();
            } else {
                token.push_back(ch);
            }
        }
        out.push_back(token);
        return out;
    }
}

jxx::Ptr<Locale> Locale::ROOT    = jxx::NEW<Locale>(emptyString(), emptyString(), emptyString(), emptyString());
jxx::Ptr<Locale> Locale::US      = Locale::intern(StringPool::intern("en"), emptyString(), StringPool::intern("US"), emptyString());
jxx::Ptr<Locale> Locale::UK      = Locale::intern(StringPool::intern("en"), emptyString(), StringPool::intern("GB"), emptyString());
jxx::Ptr<Locale> Locale::CANADA  = Locale::intern(StringPool::intern("en"), emptyString(), StringPool::intern("CA"), emptyString());
jxx::Ptr<Locale> Locale::FRANCE  = Locale::intern(StringPool::intern("fr"), emptyString(), StringPool::intern("FR"), emptyString());
jxx::Ptr<Locale> Locale::GERMANY = Locale::intern(StringPool::intern("de"), emptyString(), StringPool::intern("DE"), emptyString());
jxx::Ptr<Locale> Locale::JAPAN   = Locale::intern(StringPool::intern("ja"), emptyString(), StringPool::intern("JP"), emptyString());
jxx::Ptr<Locale> Locale::KOREA   = Locale::intern(StringPool::intern("ko"), emptyString(), StringPool::intern("KR"), emptyString());

jxx::Ptr<jxx::lang::String> Locale::normalizeLanguage(const jxx::Ptr<jxx::lang::String>& value) {
    return value ? value->toLowerCase() : emptyString();
}

jxx::Ptr<jxx::lang::String> Locale::normalizeScript(const jxx::Ptr<jxx::lang::String>& value) {
    if (!value || value->isEmpty()) return emptyString();
    std::string s = cstr(value->toLowerCase());
    if (!s.empty()) {
        s[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(s[0])));
    }
    return jxx::lang::String::valueOf(s.c_str());
}

jxx::Ptr<jxx::lang::String> Locale::normalizeCountry(const jxx::Ptr<jxx::lang::String>& value) {
    return value ? value->toUpperCase() : emptyString();
}

jxx::Ptr<jxx::lang::String> Locale::normalizeVariant(const jxx::Ptr<jxx::lang::String>& value) {
    return value ? value : emptyString();
}

Locale::Locale(const jxx::Ptr<jxx::lang::String>& language)
    : language_(normalizeLanguage(language)), script_(emptyString()), country_(emptyString()), variant_(emptyString()) {}

Locale::Locale(const jxx::Ptr<jxx::lang::String>& language,
               const jxx::Ptr<jxx::lang::String>& country)
    : language_(normalizeLanguage(language)), script_(emptyString()), country_(normalizeCountry(country)), variant_(emptyString()) {}

Locale::Locale(const jxx::Ptr<jxx::lang::String>& language,
               const jxx::Ptr<jxx::lang::String>& country,
               const jxx::Ptr<jxx::lang::String>& variant)
    : language_(normalizeLanguage(language)), script_(emptyString()), country_(normalizeCountry(country)), variant_(normalizeVariant(variant)) {}

Locale::Locale(const jxx::Ptr<jxx::lang::String>& language,
               const jxx::Ptr<jxx::lang::String>& script,
               const jxx::Ptr<jxx::lang::String>& country,
               const jxx::Ptr<jxx::lang::String>& variant)
    : language_(normalizeLanguage(language)), script_(normalizeScript(script)), country_(normalizeCountry(country)), variant_(normalizeVariant(variant)) {}

jxx::Ptr<Locale> Locale::intern(const jxx::Ptr<jxx::lang::String>& language,
                                const jxx::Ptr<jxx::lang::String>& country,
                                const jxx::Ptr<jxx::lang::String>& variant) {
    return LocaleCache::get(language, emptyString(), country, variant);
}

jxx::Ptr<Locale> Locale::intern(const jxx::Ptr<jxx::lang::String>& language,
                                const jxx::Ptr<jxx::lang::String>& script,
                                const jxx::Ptr<jxx::lang::String>& country,
                                const jxx::Ptr<jxx::lang::String>& variant) {
    return LocaleCache::get(language, script, country, variant);
}

jxx::Ptr<Locale> Locale::getDefault() { return US; }

jxx::Ptr<Locale> Locale::forLanguageTag(const jxx::Ptr<jxx::lang::String>& languageTag) {
    if (!languageTag || languageTag->isEmpty()) return ROOT;

    std::vector<std::string> parts = split(cstr(languageTag), '-');
    if (parts.empty()) return ROOT;

    auto language = jxx::lang::String::valueOf(parts[0].c_str());
    auto script = emptyString();
    auto country = emptyString();
    auto variant = emptyString();

    for (std::size_t i = 1; i < parts.size(); ++i) {
        const std::string& token = parts[i];
        if ((token.size() == 4U) && script->isEmpty()) {
            script = jxx::lang::String::valueOf(token.c_str());
            continue;
        }
        if ((token.size() == 2U || token.size() == 3U) && country->isEmpty()) {
            country = jxx::lang::String::valueOf(token.c_str());
            continue;
        }
        if (!variant->isEmpty()) {
            variant = variant->concat(jxx::lang::String::valueOf("_"))->concat(jxx::lang::String::valueOf(token.c_str()));
        } else {
            variant = jxx::lang::String::valueOf(token.c_str());
        }
    }

    return Locale::intern(language, script, country, variant);
}

jxx::Ptr<jxx::lang::String> Locale::getLanguage() const { return language_; }
jxx::Ptr<jxx::lang::String> Locale::getScript() const { return script_; }
jxx::Ptr<jxx::lang::String> Locale::getCountry() const { return country_; }
jxx::Ptr<jxx::lang::String> Locale::getVariant() const { return variant_; }

jxx::Ptr<jxx::lang::String> Locale::getISO3Language() const { return LocaleData::getISO3Language(language_); }
jxx::Ptr<jxx::lang::String> Locale::getISO3Country() const { return LocaleData::getISO3Country(country_); }
jxx::Ptr<jxx::lang::String> Locale::getDisplayLanguage() const { return LocaleData::getLanguageName(language_); }
jxx::Ptr<jxx::lang::String> Locale::getDisplayCountry() const { return LocaleData::getCountryName(country_); }

jxx::Ptr<jxx::lang::String> Locale::getDisplayName() const {
    std::vector<std::string> qualifiers;
    if (script_ && !script_->isEmpty()) qualifiers.push_back(cstr(script_));
    if (country_ && !country_->isEmpty()) qualifiers.push_back(cstr(getDisplayCountry()));
    if (variant_ && !variant_->isEmpty()) qualifiers.push_back(cstr(variant_));

    std::ostringstream os;
    if (language_ && !language_->isEmpty()) {
        os << cstr(getDisplayLanguage());
    }
    if (!qualifiers.empty()) {
        if (language_ && !language_->isEmpty()) os << " ";
        os << "(";
        for (std::size_t i = 0; i < qualifiers.size(); ++i) {
            if (i != 0U) os << ", ";
            os << qualifiers[i];
        }
        os << ")";
    }
    return jxx::lang::String::valueOf(os.str().c_str());
}

jxx::Ptr<jxx::lang::String> Locale::toLanguageTag() const {
    std::vector<std::string> parts;
    if (language_ && !language_->isEmpty()) parts.push_back(cstr(language_));
    if (script_ && !script_->isEmpty()) parts.push_back(cstr(script_));
    if (country_ && !country_->isEmpty()) parts.push_back(cstr(country_));
    if (variant_ && !variant_->isEmpty()) {
        for (const auto& v : split(cstr(variant_), '_')) parts.push_back(v);
    }
    return join(parts, "-");
}

jxx::Ptr<jxx::lang::String> Locale::toString() const {
    std::vector<std::string> parts;
    parts.push_back(cstr(language_));
    if (script_ && !script_->isEmpty()) parts.push_back(cstr(script_));
    if (country_ && !country_->isEmpty()) parts.push_back(cstr(country_));
    if (variant_ && !variant_->isEmpty()) parts.push_back(cstr(variant_));
    return join(parts, "_");
}

jxx::lang::jbool Locale::equals(jxx::Ptr<jxx::lang::Object> other) const {
    if (other.get() == this) return true;
    auto o = std::dynamic_pointer_cast<Locale>(other);
    if (!o) return false;
    return language_->equals(o->language_) && script_->equals(o->script_) && country_->equals(o->country_) && variant_->equals(o->variant_);
}

jxx::lang::jint Locale::hashCode() const {
    jxx::lang::jint result = language_ ? language_->hashCode() : 0;
    result = 31 * result + (script_ ? script_->hashCode() : 0);
    result = 31 * result + (country_ ? country_->hashCode() : 0);
    result = 31 * result + (variant_ ? variant_->hashCode() : 0);
    return result;
}

} // namespace jxx::util
