#pragma once
#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"
#include "jxx.lang.CharSequence.h"
#include "io/jxx.io.Serializable.h"

namespace jxx::lang {

class StringBuilder : public Object, public jxx::io::Serializable, public CharSequence {
public:
    StringBuilder();
    explicit StringBuilder(jint capacity);
    explicit StringBuilder(jxx::Ptr<String> s);

    jint length() const;
    jint capacity() const;
    void ensureCapacity(jint minimumCapacity);
    void setLength(jint newLength);

    StringBuilder& append(jxx::Ptr<String> s);
    StringBuilder& append(jxx::Ptr<Object> o);
    StringBuilder& append(jchar c);
    StringBuilder& append(jint i);
    StringBuilder& append(jlong l);
    StringBuilder& append(jfloat f);
    StringBuilder& append(jdouble d);
    StringBuilder& append(jbool b);
    StringBuilder& append(jxx::Ptr<CharArray> chars);
    StringBuilder& append(jxx::Ptr<CharArray> chars, jint off, jint len);

    jxx::Ptr<String> toString() const override;

private:
    std::u16string buf_;
};

}