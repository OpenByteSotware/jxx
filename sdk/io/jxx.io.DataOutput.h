#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::io
{
    /**
     * Java 8 parity: java.io.DataOutput
     */
    class DataOutput
    {
    public:
        virtual ~DataOutput() = default;

    public:
        virtual void write(jxx::lang::jint b) = 0;
        virtual void write(const jxx::lang::ByteArray b) = 0;
        virtual void write(const jxx::lang::ByteArray b,
                           jxx::lang::jint off,
                           jxx::lang::jint len) = 0;

        virtual void writeBoolean(jxx::lang::jbool v) = 0;
        virtual void writeByte(jxx::lang::jint v) = 0;
        virtual void writeShort(jxx::lang::jint v) = 0;
        virtual void writeChar(jxx::lang::jint v) = 0;
        virtual void writeInt(jxx::lang::jint v) = 0;
        virtual void writeLong(jxx::lang::jlong v) = 0;
        virtual void writeFloat(jxx::lang::jfloat v) = 0;
        virtual void writeDouble(jxx::lang::jdouble v) = 0;
        virtual void writeBytes(jxx::Ptr<jxx::lang::String> s) = 0;
        virtual void writeChars(jxx::Ptr<jxx::lang::String> s) = 0;
        virtual void writeUTF(jxx::Ptr<jxx::lang::String> s) = 0;
    };
}
