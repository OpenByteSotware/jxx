#include "jxx.lang.Charset.h"

#include <algorithm>
#include <stdexcept>

namespace jxx::lang {

static std::string to_upper_ascii(std::string s) {
    for (auto& c : s) if (c >= 'a' && c <= 'z') c = char(c - 'a' + 'A');
    return s;
}

Charset::Charset(Kind k) : kind_(k) {}

jxx::Ptr<Charset> Charset::defaultCharset() {
    static jxx::Ptr<Charset> cs = std::make_shared<Charset>(Kind::UTF8);
    return cs;
}

jxx::Ptr<Charset> Charset::forName(jxx::Ptr<String> n) {
    if (!n) throw std::invalid_argument("NullPointerException: Charset.forName(null)");

    std::string name = to_upper_ascii(n->utf8());
    if (name == "UTF-8" || name == "UTF8") return std::make_shared<Charset>(Kind::UTF8);
    if (name == "US-ASCII" || name == "ASCII") return std::make_shared<Charset>(Kind::ASCII);
    if (name == "ISO-8859-1" || name == "ISO8859-1" || name == "LATIN1") return std::make_shared<Charset>(Kind::ISO_8859_1);

    throw std::invalid_argument("UnsupportedCharsetException");
}

jxx::Ptr<String> Charset::name() const {
    switch (kind_) {
        case Kind::UTF8: return std::make_shared<String>("UTF-8");
        case Kind::ASCII: return std::make_shared<String>("US-ASCII");
        case Kind::ISO_8859_1: return std::make_shared<String>("ISO-8859-1");
    }
    return std::make_shared<String>("UTF-8");
}

jxx::lang::ByteArray Charset::encode(const jxx::Ptr<String> s) const {
    if (!s) throw std::invalid_argument("NullPointerException: Charset.encode(null)");

    if (kind_ == Kind::UTF8) {
        return s->getBytes();
    }

    auto u = s->utf16();
    auto out = jxx::NEW<jxx::lang::ByteArrayType>((std::uint32_t)u.size());

    if (kind_ == Kind::ASCII) {
        for (std::size_t i = 0; i < u.size(); ++i) {
            char16_t c = u[i];
            (*out)[(jint)i] = (jbyte)((c <= 0x7F) ? c : '?');
        }
        return out;
    }

    // ISO-8859-1
    for (std::size_t i = 0; i < u.size(); ++i) {
        char16_t c = u[i];
        (*out)[(jint)i] = (jbyte)((c <= 0xFF) ? c : '?');
    }
    return out;
}

jxx::Ptr<String> Charset::decode(const jxx::lang::ByteArray bytes) const {
    if (!bytes) throw std::invalid_argument("NullPointerException: Charset.decode(null)");

    if (kind_ == Kind::UTF8) {
        std::string s;
        s.resize(bytes->length);
        for (std::uint32_t i = 0; i < bytes->length; ++i) {
            s[i] = (char)(unsigned char)(*bytes)[(jint)i];
        }
        return jxx::NEW<String>(s);
    }

    std::u16string u;
    u.resize(bytes->length);
    for (std::uint32_t i = 0; i < bytes->length; ++i) {
        unsigned char b = (unsigned char)(*bytes)[(jint)i];
        if (kind_ == Kind::ASCII && b > 0x7F) b = '?';
        u[i] = (char16_t)b;
    }
    return std::make_shared<String>(u);
}

jbool Charset::equals(jxx::Ptr<Object> o) const {
    auto other = std::dynamic_pointer_cast<Charset>(o);
    return other && other->kind_ == kind_;
}

jint Charset::hashCode() const {
    return (jint)kind_;
}

} // namespace jxx::lang
