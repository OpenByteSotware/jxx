#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"


namespace jxx::lang {
class String;
}

namespace jxx::util {
    class Locale;

class PluralRules : public jxx::lang::Object {
private:
    jxx::Ptr<Locale> locale_;

public:
    explicit PluralRules(const jxx::Ptr<Locale>& locale);

    static jxx::Ptr<PluralRules> forLocale(const jxx::Ptr<Locale>& locale);
    jxx::Ptr<jxx::lang::String> select(jxx::lang::jint number) const;
};

} // namespace jxx::util
