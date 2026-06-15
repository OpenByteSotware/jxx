#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "jxx.io.Closeable.h"
#include "jxx.io.DataInput.h"

namespace jxx::io
{
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
