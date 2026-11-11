#include "util/jxx.util.LocaleBuilder.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.StringPool.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.IllformedLocaleException.h"

#include <cctype>

namespace jxx::util {
namespace {

bool allAlpha(const std::string& value) {
    for (unsigned char ch : value) {
        if (!std::isalpha(ch)) return false;
    }
    return true;
}

bool allDigit(const std::string& value) {
    for (unsigned char ch : value) {
        if (!std::isdigit(ch)) return false;
    }
    return true;
}

bool allAlphaNumeric(const std::string& value) {
    for (unsigned char ch : value) {
        if (!std::isalnum(ch)) return false;
    }
    return true;
}

[[noreturn]] void illformed(const char* field, ::jxx::lang::jint index = 0) {
    throw IllformedLocaleException(
        ::jxx::NEW<::jxx::lang::String>(
            std::string("Ill-formed ") + field),
        index);
}

void validateLanguage(const ::jxx::Ptr<::jxx::lang::String>& value) {
    const auto text = value->utf8();
    if (!text.empty() &&
        (text.size() < 2U || text.size() > 8U || !allAlpha(text))) {
        illformed("language");
    }
}

void validateScript(const ::jxx::Ptr<::jxx::lang::String>& value) {
    const auto text = value->utf8();
    if (!text.empty() && (text.size() != 4U || !allAlpha(text))) {
        illformed("script");
    }
}

void validateRegion(const ::jxx::Ptr<::jxx::lang::String>& value) {
    const auto text = value->utf8();
    if (!text.empty() &&
        !((text.size() == 2U && allAlpha(text)) ||
          (text.size() == 3U && allDigit(text)))) {
        illformed("region");
    }
}

void validateVariant(const ::jxx::Ptr<::jxx::lang::String>& value) {
    const auto text = value->utf8();
    if (text.empty()) return;
    std::size_t start = 0U;
    while (start <= text.size()) {
        const auto end = text.find_first_of("_-", start);
        const auto part = text.substr(start,
            end == std::string::npos ? std::string::npos : end - start);
        const bool valid =
            (part.size() >= 5U && part.size() <= 8U && allAlphaNumeric(part)) ||
            (part.size() == 4U && std::isdigit(static_cast<unsigned char>(part[0])) &&
             allAlphaNumeric(part));
        if (!valid) illformed("variant", static_cast<::jxx::lang::jint>(start));
        if (end == std::string::npos) break;
        start = end + 1U;
    }
}

} // namespace


LocaleBuilder::LocaleBuilder()
    : language_(StringPool::intern("")),
      script_(StringPool::intern("")),
      region_(StringPool::intern("")),
      variant_(StringPool::intern("")) {}

::jxx::Ptr<LocaleBuilder> LocaleBuilder::setLanguage(const ::jxx::Ptr<::jxx::lang::String>& language) {
    if (language == nullptr) throw ::jxx::lang::NullPointerException();
    validateLanguage(language);
    language_ = language;
    return getThis<LocaleBuilder>();
}

::jxx::Ptr<LocaleBuilder> LocaleBuilder::setScript(const ::jxx::Ptr<::jxx::lang::String>& script) {
    if (script == nullptr) throw ::jxx::lang::NullPointerException();
    validateScript(script);
    script_ = script;
    return getThis<LocaleBuilder>();
}

::jxx::Ptr<LocaleBuilder> LocaleBuilder::setRegion(const ::jxx::Ptr<::jxx::lang::String>& region) {
    if (region == nullptr) throw ::jxx::lang::NullPointerException();
    validateRegion(region);
    region_ = region;
    return getThis<LocaleBuilder>();
}

::jxx::Ptr<LocaleBuilder> LocaleBuilder::setVariant(const ::jxx::Ptr<::jxx::lang::String>& variant) {
    if (variant == nullptr) throw ::jxx::lang::NullPointerException();
    validateVariant(variant);
    variant_ = variant;
    return getThis<LocaleBuilder>();
}

::jxx::Ptr<LocaleBuilder> LocaleBuilder::clear() {
    language_ = StringPool::intern("");
    script_ = StringPool::intern("");
    region_ = StringPool::intern("");
    variant_ = StringPool::intern("");
    return getThis<LocaleBuilder>();
}

::jxx::Ptr<Locale> LocaleBuilder::build() {
    return Locale::intern(language_, script_, region_, variant_);
}

} // namespace jxx::util
