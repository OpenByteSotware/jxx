#pragma once
#include <locale>
#include <string>
#include "jxx_types.h"
#include "jxx.lang.Object.h"

namespace jxx::lang {

    class String;

// Java 8 semantic Locale (subset) backed by std::locale.
// NOTE: Java Locale is richer; this provides parity for common operations.
class Locale final : public Object {
public:
    Locale();
    Locale(jxx::Ptr<String> language, jxx::Ptr<String> country);

    static jxx::Ptr<Locale> getDefault();
    static jxx::Ptr<Locale> ROOT();
    static jxx::Ptr<Locale> US();

    jxx::Ptr<String> getLanguage() const;
    jxx::Ptr<String> getCountry() const;

    //jxx::Ptr<String> toString() const override;
    jbool equals(jxx::Ptr<Object> o) const override;
    jint hashCode() const override;

    const std::locale& cppLocale() const noexcept { return loc_; }

	jxx::Ptr<String> toString() const override;

private:
    std::string language_;
    std::string country_;
    std::locale loc_;

    static std::string to_utf8_(jxx::Ptr<String> s);
    static jxx::Ptr<String> from_utf8_(const std::string& s);
};

} // namespace jxx::lang
