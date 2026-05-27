#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::lang {
    class String;
    class CharSequence;
}

namespace jxx::io {

/**
 * Java 8 parity: java.io.Writer
 *
 * Note: Java Writer implements Appendable. We model append(...) methods here.
 */
class IOHelper {
public:
    virtual ~IOHelper() = default;

    static void checkBounds_(const jxx::lang::CharArray cbuf, jxx::lang::jint off, jxx::lang::jint len);
    static void checkStringBounds_(const jxx::Ptr<jxx::lang::String> s, jxx::lang::jint off, jxx::lang::jint len);   
    static void checkBounds_(const jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len);
};

} // namespace jxx::io
