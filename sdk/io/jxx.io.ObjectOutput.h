#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "jxx.io.Closeable.h"
#include "jxx.io.Flushable.h"
#include "jxx.io.DataOutput.h"

namespace jxx::io
{
    /**
     * Java 8 parity: java.io.ObjectOutput
     */
    class ObjectOutput
        : public DataOutput
        , public Flushable
        , public Closeable
    {
    public:
        ~ObjectOutput() override = default;

    public:
        virtual void write(jxx::lang::jint b) = 0;
        virtual void write(const jxx::lang::ByteArray b) = 0;
        virtual void write(const jxx::lang::ByteArray b,
                           jxx::lang::jint off,
                           jxx::lang::jint len) = 0;

        virtual void flush() override = 0;
        virtual void close() override = 0;

        virtual void writeObject(jxx::Ptr<jxx::lang::Object> obj) = 0;
    };
}
