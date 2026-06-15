#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.buildin_array.h"
#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.DataInput.h"

namespace jxx::io
{
    /**
     * Java 8 parity: java.io.ObjectInput
     */
    class ObjectInput
        : public DataInput
        , public Closeable
    {
    public:
        ~ObjectInput() override = default;

    public:
        virtual jxx::lang::jint read() = 0;
        virtual jxx::lang::jint read(const jxx::lang::ByteArray b) = 0;
        virtual jxx::lang::jint read(const jxx::lang::ByteArray b,
                                     jxx::lang::jint off,
                                     jxx::lang::jint len) = 0;

        virtual jxx::lang::jlong skip(jxx::lang::jlong n) = 0;
        virtual jxx::lang::jint available() = 0;
        virtual void close() override = 0;

        virtual jxx::Ptr<jxx::lang::Object> readObject() = 0;
    };
}
