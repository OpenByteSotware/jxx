#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::lang
{
    class String;
}

namespace jxx::io
{
    /**
     * Java 8 parity: java.io.DataInput
     */
    class DataInput
    {
    public:
        virtual ~DataInput() = default;

    public:
        virtual void readFully(const jxx::lang::ByteArray b) = 0;
        virtual void readFully(const jxx::lang::ByteArray b,
                               jxx::lang::jint off,
                               jxx::lang::jint len) = 0;

        virtual jxx::lang::jint skipBytes(jxx::lang::jint n) = 0;

        virtual jxx::lang::jbool readBoolean() = 0;
        virtual jxx::lang::jbyte readByte() = 0;
        virtual jxx::lang::jint readUnsignedByte() = 0;
        virtual jxx::lang::jshort readShort() = 0;
        virtual jxx::lang::jint readUnsignedShort() = 0;
        virtual jxx::lang::jchar readChar() = 0;
        virtual jxx::lang::jint readInt() = 0;
        virtual jxx::lang::jlong readLong() = 0;
        virtual jxx::lang::jfloat readFloat() = 0;
        virtual jxx::lang::jdouble readDouble() = 0;
        virtual jxx::Ptr<jxx::lang::String> readLine() = 0;
        virtual jxx::Ptr<jxx::lang::String> readUTF() = 0;
    };
}
