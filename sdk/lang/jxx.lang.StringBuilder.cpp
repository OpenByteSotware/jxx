#include "jxx.lang.StringBuilder.h"
#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.StringIndexOutOfBoundsException.h"
#include <sstream>
#include <locale>

namespace jxx::lang {

StringBuilder::StringBuilder() = default;
StringBuilder::StringBuilder(jint cap) { if (cap > 0) buf_.reserve((size_t)cap); }
StringBuilder::StringBuilder(jxx::Ptr<String> s) { if (s) buf_ = s->utf16(); }

jint StringBuilder::length() const { return (jint)buf_.size(); }
jint StringBuilder::capacity() const { return (jint)buf_.capacity(); }
void StringBuilder::ensureCapacity(jint m) { if (m > (jint)buf_.capacity()) buf_.reserve((size_t)m); }

void StringBuilder::setLength(jint newLen) {
    if (newLen < 0) throw StringIndexOutOfBoundsException(std::make_shared<String>("negative length"));
    buf_.resize((size_t)newLen, 0);
}

StringBuilder& StringBuilder::append(jxx::Ptr<String> s) {
    if (!s) { buf_ += u"null"; return *this; }
    buf_ += s->utf16();
    return *this;
}

StringBuilder& StringBuilder::append(jxx::Ptr<Object> o) {
    if (!o) { buf_ += u"null"; return *this; }
    return append(o->toString());
}

StringBuilder& StringBuilder::append(jchar c) { buf_.push_back((char16_t)c); return *this; }
StringBuilder& StringBuilder::append(jbool b) { return append(std::make_shared<String>(b ? "true" : "false")); }
StringBuilder& StringBuilder::append(jint i) { return append(std::make_shared<String>(std::to_string(i).c_str())); }
StringBuilder& StringBuilder::append(jlong l) { return append(std::make_shared<String>(std::to_string((long long)l).c_str())); }

StringBuilder& StringBuilder::append(jfloat f) {
    std::ostringstream oss; oss.imbue(std::locale::classic()); oss << f;
    return append(std::make_shared<String>(oss.str().c_str()));
}
StringBuilder& StringBuilder::append(jdouble d) {
    std::ostringstream oss; oss.imbue(std::locale::classic()); oss << d;
    return append(std::make_shared<String>(oss.str().c_str()));
}

StringBuilder& StringBuilder::append(jxx::Ptr<CharArray> chars) {
    if (!chars) throw NullPointerException(std::make_shared<String>("null"));
    for (std::uint32_t i = 0; i < chars->length; ++i) buf_.push_back((char16_t)(*chars)[(jint)i]);
    return *this;
}
StringBuilder& StringBuilder::append(jxx::Ptr<CharArray> chars, jint off, jint len) {
    if (!chars) throw NullPointerException(std::make_shared<String>("null"));
    if (off < 0 || len < 0 || (std::uint32_t)(off + len) > chars->length)
        throw StringIndexOutOfBoundsException(std::make_shared<String>("range"));
    for (jint i = 0; i < len; ++i) buf_.push_back((char16_t)(*chars)[off + i]);
    return *this;
}

jxx::Ptr<String> StringBuilder::toString() const {
    // If your String supports constructing from CharArray efficiently, use that.
    auto arr = std::make_shared<CharArray>((std::uint32_t)buf_.size());
    for (std::size_t i = 0; i < buf_.size(); ++i) (*arr)[(jint)i] = (jchar)buf_[i];
    return std::make_shared<String>(arr);
}

}