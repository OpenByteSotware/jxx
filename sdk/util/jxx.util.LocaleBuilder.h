#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Locale.h"

namespace jxx::lang {
class String;
}

namespace jxx::util {

class LocaleBuilder : public jxx::lang::Object {
private:
    jxx::Ptr<jxx::lang::String> language_;
    jxx::Ptr<jxx::lang::String> script_;
    jxx::Ptr<jxx::lang::String> region_;
    jxx::Ptr<jxx::lang::String> variant_;

public:
    LocaleBuilder();

    jxx::Ptr<LocaleBuilder> setLanguage(const jxx::Ptr<jxx::lang::String>& language);
    jxx::Ptr<LocaleBuilder> setScript(const jxx::Ptr<jxx::lang::String>& script);
    jxx::Ptr<LocaleBuilder> setRegion(const jxx::Ptr<jxx::lang::String>& region);
    jxx::Ptr<LocaleBuilder> setVariant(const jxx::Ptr<jxx::lang::String>& variant);
    jxx::Ptr<LocaleBuilder> clear();
    jxx::Ptr<Locale> build();
};

} // namespace jxx::util
