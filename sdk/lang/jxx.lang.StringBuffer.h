#pragma once
#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"
#include "jxx.lang.StringBuilder.h"

namespace jxx::lang {

// Java StringBuffer is synchronized. Implemented by delegating to StringBuilder inside synchronized blocks.
class StringBuffer : public Object {
public:
    StringBuffer();
    explicit StringBuffer(jint capacity);
    explicit StringBuffer(jxx::Ptr<String> s);

    jint length() const;
    jint capacity() const;
    void ensureCapacity(jint minimumCapacity);
    void setLength(jint newLength);

    StringBuffer& append(jxx::Ptr<String> s);
    StringBuffer& append(jxx::Ptr<Object> o);
    StringBuffer& append(jchar c);
    StringBuffer& append(jint i);
    StringBuffer& append(jlong l);
    StringBuffer& append(jfloat f);
    StringBuffer& append(jdouble d);
    StringBuffer& append(jbool b);
    StringBuffer& append(jxx::Ptr<CharArray> chars);
    StringBuffer& append(jxx::Ptr<CharArray> chars, jint off, jint len);

    jxx::Ptr<String> toString() const override;

private:
    StringBuilder sb_;
};

}