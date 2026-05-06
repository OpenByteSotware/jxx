#include "jxx.lang.StringBuffer.h"

namespace jxx::lang {

StringBuffer::StringBuffer() = default;
StringBuffer::StringBuffer(jint cap) : sb_(cap) {}
StringBuffer::StringBuffer(jxx::Ptr<String> s) : sb_(s) {}

jint StringBuffer::length() const { return this->synchronized([&urn sb_.length(); }); }
jint StringBuffer::capacity() const { return this->synchronized([& return sb_.capacity(); }); }
void StringBuffer::ensureCapacity(jint m) { this->synchronized([& sb_.ensureCapacity(m); }); }
void StringBuffer::setLength(jint n) { this->synchronized([& sb_.setLength(n); }); }

StringBuffer& StringBuffer::append(jxx::Ptr<String> s) { this->synchronized([& sb_.append(s); }); return *this; }
StringBuffer& StringBuffer::append(jxx::Ptr<Object> o) { this->synchronized([& sb_.append(o); }); return *this; }
StringBuffer& StringBuffer::append(jchar c) { this->synchronized([& sb_.append(c); }); return *this; }
StringBuffer& StringBuffer::append(jint i) { this->synchronized([& sb_.append(i); }); return *this; }
StringBuffer& StringBuffer::append(jlong l) { this->synchronized([& sb_.append(l); }); return *this; }
StringBuffer& StringBuffer::append(jfloat f) { this->synchronized([& sb_.append(f); }); return *this; }
StringBuffer& StringBuffer::append(jdouble d) { this->synchronized([& sb_.append(d); }); return *this; }
StringBuffer& StringBuffer::append(jbool b) { this->synchronized(&{ sb_.append(b); }); return *this; }
StringBuffer& StringBuffer::append(jxx::Ptr<CharArray> c) { this->synchronized([& sb_.append(c); }); return *this; }
StringBuffer& StringBuffer::append(jxx::Ptr<CharArray> c, jint o, jint l) { this->synchronized([& sb_.append(c,o,l); }); return *this; }

jxx::Ptr<String> StringBuffer::toString() const { return this->synchronized([& return sb_.toString(); }); }

}