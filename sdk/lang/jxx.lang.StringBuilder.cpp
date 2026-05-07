#include <sstream>
#include <locale>
#include <sstream>
#include "jxx_types.h"
#include "jxx.lang.StringBuilder.h"
#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.StringIndexOutOfBoundsException.h"

namespace jxx::lang {

    jxx::Ptr<StringBuilder> StringBuilder::make() {
        return JXX_NEW<StringBuilder>();
    }
    jxx::Ptr<StringBuilder> StringBuilder::make(jint capacity) {
        return JXX_NEW<StringBuilder>(capacity);
    }
    jxx::Ptr<StringBuilder> StringBuilder::make(jxx::Ptr<String> s) {
        return JXX_NEW<StringBuilder>(s);
    }

    StringBuilder::StringBuilder(jint cap) {
        if (cap > 0) buf_.reserve((size_t)cap);
    }
    StringBuilder::StringBuilder(jxx::Ptr<String> s) {
        if (s) buf_ = s->utf16();
    }

    jxx::Ptr<StringBuilder> StringBuilder::append(jxx::Ptr<String> s) {
        if (!s) buf_ += u"null";
        else buf_ += s->utf16();
        return getThis<StringBuilder>();
    }

    jxx::Ptr<StringBuilder> StringBuilder::append(jxx::Ptr<Object> o) {
        if (!o) buf_ += u"null";
        else buf_ += o->toString()->utf16();
        return getThis<StringBuilder>();
    }

    jxx::Ptr<StringBuilder> StringBuilder::append(jint v) {
        return append(JXX_NEW<String>(std::to_string(v).c_str()));
    }
    jxx::Ptr<StringBuilder> StringBuilder::append(jlong v) {
        return append(JXX_NEW<String>(std::to_string((long long)v).c_str()));
    }
    jxx::Ptr<StringBuilder> StringBuilder::append(jbool v) {
        return append(JXX_NEW<String>(v ? "true" : "false"));
    }
    jxx::Ptr<StringBuilder> StringBuilder::append(jchar v) {
        buf_.push_back((char16_t)v);
        return shared_from_this();
    }

    jxx::Ptr<std::string> StringBuilder::toString() const {
        auto arr = JXX_NEW<CharArray>((jint)buf_.size());
        for (jint i = 0; i < (jint)buf_.size(); ++i)
            (*arr)[i] = (jchar)buf_[i];
        return JXX_NEW<std::string>(arr);
    }

} // namespace jxx::lang