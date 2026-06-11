#pragma once

#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::util {

class LocaleData {
public:
    static jxx::Ptr<jxx::lang::String> getLanguageName(const jxx::Ptr<jxx::lang::String>& language);
    static jxx::Ptr<jxx::lang::String> getCountryName(const jxx::Ptr<jxx::lang::String>& country);
    static jxx::Ptr<jxx::lang::String> getISO3Language(const jxx::Ptr<jxx::lang::String>& language);
    static jxx::Ptr<jxx::lang::String> getISO3Country(const jxx::Ptr<jxx::lang::String>& country);
};

} // namespace jxx::util
