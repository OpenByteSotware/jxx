#include "jxx.lang.Locale.h"

#include <functional>

namespace jxx::lang {

std::string Locale::to_utf8_(jxx::Ptr<String> s) {
    if (!s) return std::string{};
    return s->utf8();
}

jxx::Ptr<String> Locale::from_utf8_(const std::string& s) {
    return std::make_shared<String>(s);
}

Locale::Locale() : language_(), country_(), loc_(std::locale()) {}

Locale::Locale(jxx::Ptr<String> language, jxx::Ptr<String> country)
    : language_(to_utf8_(language)), country_(to_utf8_(country)), loc_(std::locale()) {}

jxx::Ptr<Locale> Locale::getDefault() {
    static jxx::Ptr<Locale> inst = std::make_shared<Locale>();
    return inst;
}

jxx::Ptr<Locale> Locale::ROOT() {
    static jxx::Ptr<Locale> root = std::make_shared<Locale>(std::make_shared<String>(""), std::make_shared<String>(""));
    return root;
}

jxx::Ptr<Locale> Locale::US() {
    static jxx::Ptr<Locale> us = std::make_shared<Locale>(std::make_shared<String>("en"), std::make_shared<String>("US"));
    return us;
}

jxx::Ptr<String> Locale::getLanguage() const { return from_utf8_(language_); }

jxx::Ptr<String> Locale::getCountry() const { return from_utf8_(country_); }

jxx::Ptr<std::string> Locale::toString() const {
    if (language_.empty() && country_.empty()) return from_utf8_("");
    if (country_.empty()) return from_utf8_(language_);
    return from_utf8_(language_ + "_" + country_);
}

jbool Locale::equals(jxx::Ptr<Object> o) const {
    auto other = std::dynamic_pointer_cast<Locale>(o);
    if (!other) return false;
    return language_ == other->language_ && country_ == other->country_;
}

jint Locale::hashCode() const {
    std::hash<std::string> h;
    std::size_t v = h(language_);
    v = v * 31u + h(country_);
    return (jint)(v & 0x7fffffff);
}

} // namespace jxx::lang
