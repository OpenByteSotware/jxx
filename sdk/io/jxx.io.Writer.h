#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"
#include "jxx.lang.CharSequence.h"

#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.IndexOutOfBoundsException.h"

#include "jxx.io.Closeable.h"
#include "jxx.io.Flushable.h"
#include "jxx.io.IOException.h"

namespace jxx::io {

/**
 * Java 8 parity: java.io.Writer
 *
 * Note: Java Writer implements Appendable. We model append(...) methods here.
 */
class Writer : public jxx::lang::Object, public Closeable, public Flushable {
public:
    virtual ~Writer() = default;

    // abstract
    virtual void write(jxx::lang::jint c) = 0;

    // overloads
    virtual void write(jxx::Ptr<CharArray> cbuf);
    virtual void write(jxx::Ptr<CharArray> cbuf, jxx::lang::jint off, jxx::lang::jint len);

    virtual void write(jxx::Ptr<jxx::lang::String> str);
    virtual void write(jxx::Ptr<jxx::lang::String> str, jxx::lang::jint off, jxx::lang::jint len);

    // append
    virtual jxx::Ptr<Writer> append(jxx::Ptr<jxx::lang::CharSequence> csq);
    virtual jxx::Ptr<Writer> append(jxx::Ptr<jxx::lang::CharSequence> csq, jxx::lang::jint start, jxx::lang::jint end);
    virtual jxx::Ptr<Writer> append(jxx::lang::jchar c);

    // default no-op
    void flush() override;
    void close() override;

protected:
    static void checkBounds_(jxx::Ptr<CharArray> cbuf, jxx::lang::jint off, jxx::lang::jint len);
    static void checkStringBounds_(jxx::Ptr<jxx::lang::String> s, jxx::lang::jint off, jxx::lang::jint len);

    jxx::Ptr<Writer> self_();
};

} // namespace jxx::io
