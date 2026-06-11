#include "util/jxx.util.Locale.h"
#include "util/jxx.util.PluralRules.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.StringPool.h"

#include <string>

namespace jxx::util {

PluralRules::PluralRules(const jxx::Ptr<Locale>& locale) : locale_(locale ? locale : Locale::getDefault()) {}

jxx::Ptr<PluralRules> PluralRules::forLocale(const jxx::Ptr<Locale>& locale) {
    return jxx::NEW<PluralRules>(locale);
}

jxx::Ptr<jxx::lang::String> PluralRules::select(jxx::lang::jint number) const {
    std::string language = locale_->getLanguage() ? locale_->getLanguage()->utf8() : std::string();
    if (language == "fr" || language == "pt") {
        return ((number == 0) || (number == 1)) ? StringPool::intern("one") : StringPool::intern("other");
    }
    if (language == "ru") {
        jxx::lang::jint mod10 = number % 10;
        jxx::lang::jint mod100 = number % 100;
        if (mod10 == 1 && mod100 != 11) return StringPool::intern("one");
        if (mod10 >= 2 && mod10 <= 4 && !(mod100 >= 12 && mod100 <= 14)) return StringPool::intern("few");
        return StringPool::intern("other");
    }
    return (number == 1) ? StringPool::intern("one") : StringPool::intern("other");
}

} // namespace jxx::util
