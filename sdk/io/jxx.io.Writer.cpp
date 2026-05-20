#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "jxx.io.IOException.h"
#include "jxx.io.Writer.h"

namespace jxx::io {

jxx::Ptr<Writer> Writer::self_() {
    return std::static_pointer_cast<Writer>(this->thisPtr);
}

void Writer::checkBounds_(jxx::Ptr<CharArray> cbuf, jxx::lang::jint off, jxx::lang::jint len) {
    if (!cbuf) throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("cbuf"));
    if (off < 0 || len < 0 || (std::uint32_t)(off + len) > cbuf->length) {
        throw jxx::lang::IndexOutOfBoundsException(JXX_NEW<jxx::lang::String>("off/len"));
    }
}

void Writer::checkStringBounds_(jxx::Ptr<jxx::lang::String> s, jxx::lang::jint off, jxx::lang::jint len) {
    if (!s) throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("str"));
    if (off < 0 || len < 0 || off + len > s->length()) {
        throw jxx::lang::IndexOutOfBoundsException(JXX_NEW<jxx::lang::String>("off/len"));
    }
}

void Writer::write(jxx::Ptr<CharArray> cbuf) {
    if (!cbuf) throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("cbuf"));
    write(cbuf, 0, (jxx::lang::jint)cbuf->length);
}

void Writer::write(jxx::Ptr<CharArray> cbuf, jxx::lang::jint off, jxx::lang::jint len) {
    checkBounds_(cbuf, off, len);
    for (jxx::lang::jint i = 0; i < len; ++i) write((jxx::lang::jint)(*cbuf)[off + i]);
}

void Writer::write(jxx::Ptr<jxx::lang::String> str) {
    if (!str) throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("str"));
    write(str, 0, str->length());
}

void Writer::write(jxx::Ptr<jxx::lang::String> str, jxx::lang::jint off, jxx::lang::jint len) {
    checkStringBounds_(str, off, len);
    const auto& u16 = str->utf16();
    for (jxx::lang::jint i = 0; i < len; ++i) write((jxx::lang::jint)u16[(std::size_t)(off + i)]);
}

jxx::Ptr<Writer> Writer::append(jxx::Ptr<jxx::lang::CharSequence> csq) {
    if (!csq) {
        write(JXX_NEW<jxx::lang::String>("null"));
        return self_();
    }
    // write entire sequence
    auto a = JXX_NEW<CharArray>((std::uint32_t)csq->length());
    for (jxx::lang::jint i = 0; i < csq->length(); ++i) (*a)[i] = csq->charAt(i);
    write(a);
    return self_();
}

jxx::Ptr<Writer> Writer::append(jxx::Ptr<jxx::lang::CharSequence> csq, jxx::lang::jint start, jxx::lang::jint end) {
    if (!csq) csq = JXX_NEW<jxx::lang::String>("null");
    if (start < 0 || end < start || end > csq->length()) {
        throw jxx::lang::IndexOutOfBoundsException(JXX_NEW<jxx::lang::String>("start/end"));
    }
    auto a = JXX_NEW<CharArray>((std::uint32_t)(end - start));
    for (jxx::lang::jint i = 0; i < (end - start); ++i) (*a)[i] = csq->charAt(start + i);
    write(a);
    return self_();
}

jxx::Ptr<Writer> Writer::append(jxx::lang::jchar c) {
    write((jxx::lang::jint)c);
    return self_();
}

void Writer::flush() {}
void Writer::close() {}

} // namespace jxx::io
