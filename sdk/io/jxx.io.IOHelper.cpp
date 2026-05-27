#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "jxx.io.IOException.h"
#include "jxx.io.IOHelper.h"

namespace jxx::io {
    void IOHelper::checkBounds_(const jxx::lang::CharArray cbuf, jxx::lang::jint off, jxx::lang::jint len) {
        if (!cbuf) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("cbuf"));
        if (off < 0 || len < 0 || (std::uint32_t)(off + len) > cbuf->length) {
            throw jxx::lang::IndexOutOfBoundsException(jxx::NEW<jxx::lang::String>("off/len"));
        }
    }

    void IOHelper::checkStringBounds_(const jxx::Ptr<jxx::lang::String> s, jxx::lang::jint off, jxx::lang::jint len) {
        if (!s) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("str"));
        if (off < 0 || len < 0 || off + len > s->length()) {
            throw jxx::lang::IndexOutOfBoundsException(jxx::NEW<jxx::lang::String>("off/len"));
        }
    }

    void IOHelper::checkBounds_(const jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len) {
        if (!b) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("b"));
        if (off < 0 || len < 0 || (std::uint32_t)(off + len) > b->length) {
            throw jxx::lang::IndexOutOfBoundsException(jxx::NEW<jxx::lang::String>("off/len"));
        }
    }
}