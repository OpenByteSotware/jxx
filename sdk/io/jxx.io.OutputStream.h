#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Object.h"
#include "jxx.io.Closeable.h"
#include "jxx.io.Flushable.h"
#include "jxx.io.IOException.h"

namespace jxx::io {

// Java 8: java.io.OutputStream
class OutputStream : public jxx::lang::Object, public Closeable, public Flushable {
public:
    virtual ~OutputStream() = default;

    // Writes the low 8 bits.
    virtual void write(jxx::lang::jint b) = 0;

    virtual void write(jxx::Ptr<ByteArray> b);
    virtual void write(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len);

    void flush() override;
    void close() override;

protected:
    static void checkBounds_(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len);
};

} // namespace jxx::io
