#include "lang/jxx.lang.String.h"
#include "jxx.util.Locale.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::util {

    std::string Locale::to_utf8_(jxx::Ptr<jxx::lang::String> s) {
        if (!s) return std::string{};
        return s->utf8();
    }

    jxx::Ptr<jxx::lang::String> Locale::from_utf8_(const std::string& s) {
        return std::make_shared<jxx::lang::String>(s);
    }

    Locale::Locale() : language_(), country_(), loc_(std::locale()) {}

    Locale::Locale(jxx::Ptr<jxx::lang::String> language, jxx::Ptr<jxx::lang::String> country)
        : language_(to_utf8_(language)), country_(to_utf8_(country)), loc_(std::locale()) {}

    jxx::Ptr<Locale> Locale::getDefault() {
        static jxx::Ptr<Locale> inst = std::make_shared<Locale>();
        return inst;
    }

    jxx::Ptr<Locale> Locale::ROOT() {
        static jxx::Ptr<Locale> root = std::make_shared<Locale>(std::make_shared<jxx::lang::String>(""), std::make_shared<jxx::lang::String>(""));
        return root;
    }

    jxx::Ptr<Locale> Locale::US() {
        static jxx::Ptr<Locale> us = std::make_shared<Locale>(std::make_shared<jxx::lang::String>("en"), std::make_shared<jxx::lang::String>("US"));
        return us;
    }

    jxx::Ptr<jxx::lang::String> Locale::getLanguage() const { return from_utf8_(language_); }
    jxx::Ptr<jxx::lang::String> Locale::getCountry() const { return from_utf8_(country_); }

    jxx::Ptr<jxx::lang::String> Locale::toString() const {
        if (language_.empty() && country_.empty()) return from_utf8_("");
        if (country_.empty()) return from_utf8_(language_);
        return from_utf8_(language_ + "_" + country_);
    }

    jxx::lang::jbool Locale::equals(jxx::Ptr<jxx::lang::Object> o) const {
        auto other = std::dynamic_pointer_cast<Locale>(o);
        if (!other) return false;
        return language_ == other->language_ && country_ == other->country_;
    }

    jxx::lang::jint Locale::hashCode() const {
        std::hash<std::string> h;
        std::size_t v = h(language_);
        v = v * 31u + h(country_);
        return (jxx::lang::jint)(v & 0x7fffffff);
    }

} // namespace jxx::util
