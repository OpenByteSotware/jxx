#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang {
class String;
}

namespace jxx::util {

class Locale : public jxx::lang::Object {
private:
    jxx::Ptr<jxx::lang::String> language_;
    jxx::Ptr<jxx::lang::String> script_;
    jxx::Ptr<jxx::lang::String> country_;
    jxx::Ptr<jxx::lang::String> variant_;

    static jxx::Ptr<jxx::lang::String> normalizeLanguage(const jxx::Ptr<jxx::lang::String>& value);
    static jxx::Ptr<jxx::lang::String> normalizeScript(const jxx::Ptr<jxx::lang::String>& value);
    static jxx::Ptr<jxx::lang::String> normalizeCountry(const jxx::Ptr<jxx::lang::String>& value);
    static jxx::Ptr<jxx::lang::String> normalizeVariant(const jxx::Ptr<jxx::lang::String>& value);

public:
    static jxx::Ptr<Locale> ROOT;
    static jxx::Ptr<Locale> US;
    static jxx::Ptr<Locale> UK;
    static jxx::Ptr<Locale> CANADA;
    static jxx::Ptr<Locale> FRANCE;
    static jxx::Ptr<Locale> GERMANY;
    static jxx::Ptr<Locale> JAPAN;
    static jxx::Ptr<Locale> KOREA;

    Locale(const jxx::Ptr<jxx::lang::String>& language);
    Locale(const jxx::Ptr<jxx::lang::String>& language, const jxx::Ptr<jxx::lang::String>& country);
    Locale(const jxx::Ptr<jxx::lang::String>& language,
           const jxx::Ptr<jxx::lang::String>& country,
           const jxx::Ptr<jxx::lang::String>& variant);
    Locale(const jxx::Ptr<jxx::lang::String>& language,
           const jxx::Ptr<jxx::lang::String>& script,
           const jxx::Ptr<jxx::lang::String>& country,
           const jxx::Ptr<jxx::lang::String>& variant);

    static jxx::Ptr<Locale> intern(const jxx::Ptr<jxx::lang::String>& language,
                                   const jxx::Ptr<jxx::lang::String>& country,
                                   const jxx::Ptr<jxx::lang::String>& variant);
    static jxx::Ptr<Locale> intern(const jxx::Ptr<jxx::lang::String>& language,
                                   const jxx::Ptr<jxx::lang::String>& script,
                                   const jxx::Ptr<jxx::lang::String>& country,
                                   const jxx::Ptr<jxx::lang::String>& variant);

    static jxx::Ptr<Locale> getDefault();
    static jxx::Ptr<Locale> forLanguageTag(const jxx::Ptr<jxx::lang::String>& languageTag);

    jxx::Ptr<jxx::lang::String> getLanguage() const;
    jxx::Ptr<jxx::lang::String> getScript() const;
    jxx::Ptr<jxx::lang::String> getCountry() const;
    jxx::Ptr<jxx::lang::String> getVariant() const;

    jxx::Ptr<jxx::lang::String> getISO3Language() const;
    jxx::Ptr<jxx::lang::String> getISO3Country() const;
    jxx::Ptr<jxx::lang::String> getDisplayLanguage() const;
    jxx::Ptr<jxx::lang::String> getDisplayCountry() const;
    jxx::Ptr<jxx::lang::String> getDisplayName() const;

    jxx::Ptr<jxx::lang::String> toLanguageTag() const;
    jxx::Ptr<jxx::lang::String> toString() const override;
    jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> other) const override;
    jxx::lang::jint hashCode() const override;
};

} // namespace jxx::util
