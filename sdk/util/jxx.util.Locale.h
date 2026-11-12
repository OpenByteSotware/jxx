#pragma once

#include "lang/jxx_types.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang {
class String;
}

namespace jxx::util {

class LocaleBuilder;

class Locale final
    : public ::jxx::lang::ClassBase<
          Locale,
          ::jxx::lang::Object,
          ::jxx::lang::Cloneable,
          ::jxx::io::SerializableI> {
private:
    ::jxx::Ptr<::jxx::lang::String> language_;
    ::jxx::Ptr<::jxx::lang::String> script_;
    ::jxx::Ptr<::jxx::lang::String> country_;
    ::jxx::Ptr<::jxx::lang::String> variant_;

    static ::jxx::Ptr<::jxx::lang::String> normalizeLanguage(const ::jxx::Ptr<::jxx::lang::String>& value);
    static ::jxx::Ptr<::jxx::lang::String> normalizeScript(const ::jxx::Ptr<::jxx::lang::String>& value);
    static ::jxx::Ptr<::jxx::lang::String> normalizeCountry(const ::jxx::Ptr<::jxx::lang::String>& value);
    static ::jxx::Ptr<::jxx::lang::String> normalizeVariant(const ::jxx::Ptr<::jxx::lang::String>& value);

public:
    // Source-compatible nested public type: Locale.Builder.
    using Builder = LocaleBuilder;

    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        Locale, JxxSuper, ::jxx::lang::Cloneable, ::jxx::io::SerializableI>;

    static ::jxx::Ptr<Locale> ROOT;
    static ::jxx::Ptr<Locale> US;
    static ::jxx::Ptr<Locale> UK;
    static ::jxx::Ptr<Locale> CANADA;
    static ::jxx::Ptr<Locale> FRANCE;
    static ::jxx::Ptr<Locale> GERMANY;
    static ::jxx::Ptr<Locale> JAPAN;
    static ::jxx::Ptr<Locale> KOREA;
    static ::jxx::Ptr<Locale> CANADA_FRENCH;
    static ::jxx::Ptr<Locale> CHINA;
    static ::jxx::Ptr<Locale> CHINESE;
    static ::jxx::Ptr<Locale> ENGLISH;
    static ::jxx::Ptr<Locale> FRENCH;
    static ::jxx::Ptr<Locale> GERMAN;
    static ::jxx::Ptr<Locale> ITALIAN;
    static ::jxx::Ptr<Locale> ITALY;
    static ::jxx::Ptr<Locale> JAPANESE;
    static ::jxx::Ptr<Locale> KOREAN;
    static ::jxx::Ptr<Locale> PRC;
    static ::jxx::Ptr<Locale> SIMPLIFIED_CHINESE;
    static ::jxx::Ptr<Locale> TAIWAN;
    static ::jxx::Ptr<Locale> TRADITIONAL_CHINESE;

    Locale(const ::jxx::Ptr<::jxx::lang::String>& language);
    Locale(const ::jxx::Ptr<::jxx::lang::String>& language, const ::jxx::Ptr<::jxx::lang::String>& country);
    Locale(const ::jxx::Ptr<::jxx::lang::String>& language,
           const ::jxx::Ptr<::jxx::lang::String>& country,
           const ::jxx::Ptr<::jxx::lang::String>& variant);
    Locale(const ::jxx::Ptr<::jxx::lang::String>& language,
           const ::jxx::Ptr<::jxx::lang::String>& script,
           const ::jxx::Ptr<::jxx::lang::String>& country,
           const ::jxx::Ptr<::jxx::lang::String>& variant);

    static ::jxx::Ptr<Locale> intern(const ::jxx::Ptr<::jxx::lang::String>& language,
                                   const ::jxx::Ptr<::jxx::lang::String>& country,
                                   const ::jxx::Ptr<::jxx::lang::String>& variant);
    static ::jxx::Ptr<Locale> intern(const ::jxx::Ptr<::jxx::lang::String>& language,
                                   const ::jxx::Ptr<::jxx::lang::String>& script,
                                   const ::jxx::Ptr<::jxx::lang::String>& country,
                                   const ::jxx::Ptr<::jxx::lang::String>& variant);

    static ::jxx::Ptr<Locale> getDefault();
    static void setDefault(const ::jxx::Ptr<Locale>& locale);
    static ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<Locale>, 1U>>
        getAvailableLocales();
    static ::jxx::Ptr<::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>> getISOCountries();
    static ::jxx::Ptr<::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>> getISOLanguages();
    static ::jxx::Ptr<Locale> forLanguageTag(const ::jxx::Ptr<::jxx::lang::String>& languageTag);

    ::jxx::Ptr<::jxx::lang::String> getLanguage() const;
    ::jxx::Ptr<::jxx::lang::String> getScript() const;
    ::jxx::Ptr<::jxx::lang::String> getCountry() const;
    ::jxx::Ptr<::jxx::lang::String> getVariant() const;

    ::jxx::Ptr<::jxx::lang::String> getISO3Language() const;
    ::jxx::Ptr<::jxx::lang::String> getISO3Country() const;
    ::jxx::Ptr<::jxx::lang::String> getDisplayLanguage() const;
    ::jxx::Ptr<::jxx::lang::String> getDisplayLanguage(
        const ::jxx::Ptr<Locale>& displayLocale) const;
    ::jxx::Ptr<::jxx::lang::String> getDisplayScript() const;
    ::jxx::Ptr<::jxx::lang::String> getDisplayScript(
        const ::jxx::Ptr<Locale>& displayLocale) const;
    ::jxx::Ptr<::jxx::lang::String> getDisplayCountry() const;
    ::jxx::Ptr<::jxx::lang::String> getDisplayCountry(
        const ::jxx::Ptr<Locale>& displayLocale) const;
    ::jxx::Ptr<::jxx::lang::String> getDisplayVariant() const;
    ::jxx::Ptr<::jxx::lang::String> getDisplayVariant(
        const ::jxx::Ptr<Locale>& displayLocale) const;
    ::jxx::Ptr<::jxx::lang::String> getDisplayName() const;
    ::jxx::Ptr<::jxx::lang::String> getDisplayName(
        const ::jxx::Ptr<Locale>& displayLocale) const;

    ::jxx::Ptr<::jxx::lang::String> toLanguageTag() const;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
    ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& other) const override;
    ::jxx::lang::jint hashCode() const override;

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override {
        if (out == nullptr) throw ::jxx::lang::NullPointerException();
        out->writeObject(language_);
        out->writeObject(script_);
        out->writeObject(country_);
        out->writeObject(variant_);
    }
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override {
        if (in == nullptr) throw ::jxx::lang::NullPointerException();
        language_ = normalizeLanguage(::jxx::CAST<::jxx::lang::String>(in->readObject()));
        script_ = normalizeScript(::jxx::CAST<::jxx::lang::String>(in->readObject()));
        country_ = normalizeCountry(::jxx::CAST<::jxx::lang::String>(in->readObject()));
        variant_ = normalizeVariant(::jxx::CAST<::jxx::lang::String>(in->readObject()));
    }
    void readObjectNoData() override {
        const auto empty = ::jxx::NEW<::jxx::lang::String>("");
        language_ = empty;
        script_ = empty;
        country_ = empty;
        variant_ = empty;
    }

protected:
    JXX_OBJECT_CLONE(Locale)
};

} // namespace jxx::util
