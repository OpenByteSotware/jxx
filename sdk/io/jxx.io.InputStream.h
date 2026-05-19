#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Object.h"
#include "jxx.io.Closeable.h"

namespace jxx::io {

    class InputStream : public jxx::lang::Object, public Closeable {
    public:
        virtual ~InputStream() = default;

        // abstract
        virtual jxx::lang::jint read() = 0;

        // read(byte[])
        virtual jxx::lang::jint read(jxx::Ptr<ByteArray> b);

        // read(byte[], off, len)
        virtual jxx::lang::jint read(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len);

        virtual jxx::lang::jlong skip(jxx::lang::jlong n);
        virtual jxx::lang::jint available();

        void close() override;

        virtual void mark(jxx::lang::jint readlimit);
        virtual void reset();
        virtual jxx::lang::jbool markSupported();

    protected:
        static void checkBounds_(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len);
    };

} // namespace jxx::io