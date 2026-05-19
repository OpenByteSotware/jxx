#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"

#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.IndexOutOfBoundsException.h"

#include "jxx.io.Closeable.h"
#include "jxx.io.Flushable.h"
#include "jxx.io.IOException.h"

namespace jxx::io {

// Java 8: java.io.Writer
class Writer : public jxx::lang::Object, public Closeable, public Flushable {
public:
    virtual ~Writer() = default;

    // Java: void write(int c)
    virtual void write(jxx::lang::jint c) = 0;

    virtual void write(jxx::Ptr<CharArray> cbuf);
    virtual void write(jxx::Ptr<CharArray> cbuf, jxx::lang::jint off, jxx::lang::jint len);

    virtual void write(jxx::Ptr<jxx::lang::String> str);
    virtual void write(jxx::Ptr<jxx::lang::String> str, jxx::lang::jint off, jxx::lang::jint len);

    virtual jxx::Ptr<Writer> append(jxx::lang::jchar c);

    void flush() override;
    void close() override;

protected:
    static void checkBounds_(jxx::Ptr<CharArray> cbuf, jxx::lang::jint off, jxx::lang::jint len);
    static void checkStringBounds_(jxx::Ptr<jxx::lang::String> s, jxx::lang::jint off, jxx::lang::jint len);

    jxx::Ptr<Writer> self_();
};

} // namespace jxx::io
