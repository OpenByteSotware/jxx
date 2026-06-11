#pragma once

#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::util {

class Locale;

class LocaleCache {
public:
    static jxx::Ptr<Locale> get(const jxx::Ptr<jxx::lang::String>& language,
                                const jxx::Ptr<jxx::lang::String>& script,
                                const jxx::Ptr<jxx::lang::String>& country,
                                const jxx::Ptr<jxx::lang::String>& variant);
};

} // namespace jxx::util
