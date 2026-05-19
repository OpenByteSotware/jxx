#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::util {

/**
 * Minimal Java 8 parity: java.util.Locale
 * Enough for Formatter locale() and constructors.
 */
    class Locale final : public jxx::lang::Object {
    public:
        Locale();
        Locale(jxx::Ptr<jxx::lang::String> language, jxx::Ptr<jxx::lang::String> country);

        static jxx::Ptr<Locale> getDefault();
        static jxx::Ptr<Locale> ROOT();
        static jxx::Ptr<Locale> US();

        jxx::Ptr<jxx::lang::String> getLanguage() const;
        jxx::Ptr<jxx::lang::String> getCountry() const;

        //jxx::Ptr<String> toString() const override;
        jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> o) const override;
        jxx::lang::jint hashCode() const override;

        const std::locale& cppLocale() const noexcept { return loc_; }

        jxx::Ptr<jxx::lang::String> toString() const override;

    private:
        std::string language_;
        std::string country_;
        std::locale loc_;

        static std::string to_utf8_(jxx::Ptr<jxx::lang::String> s);
        static jxx::Ptr<jxx::lang::String> from_utf8_(const std::string& s);
    };


} // namespace jxx::util
