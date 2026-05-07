#pragma once
#include <string>
#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"
#include "jxx.lang.CharSequence.h"
#include "io/jxx.io.Serializable.h"

namespace jxx::lang {

class StringBuilder : public Object, public jxx::io::Serializable, public CharSequence {
public:
    static jxx::Ptr<StringBuilder> make();
    static jxx::Ptr<StringBuilder> make(jint capacity);
    static jxx::Ptr<StringBuilder> make(jxx::Ptr<String> s);

    jxx::Ptr<StringBuilder> append(jxx::Ptr<String> s);
    jxx::Ptr<StringBuilder> append(jxx::Ptr<Object> o);
    jxx::Ptr<StringBuilder> append(jint v);
    jxx::Ptr<StringBuilder> append(jlong v);
    jxx::Ptr<StringBuilder> append(jbool v);
    jxx::Ptr<StringBuilder> append(jchar v);

    jxx::Ptr<std::string> toString() const override;

private:
    StringBuilder() = default;
    explicit StringBuilder(jint cap);
    explicit StringBuilder(jxx::Ptr<String> s);

    std::u16string buf_;
};

}