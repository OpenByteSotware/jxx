#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.ObjectInput.h"

namespace jxx::lang
{
    class String;
}
namespace jxx::io
{
    class ObjectInputStream : public InputStream, public ObjectInput
    {
    public:
        explicit ObjectInputStream(jxx::Ptr<InputStream> in);
        ~ObjectInputStream() override = default;

    public:
        jxx::lang::jint read() override;
        jxx::lang::jint read(const jxx::lang::ByteArray b,
                             jxx::lang::jint off,
                             jxx::lang::jint len) override;
        jxx::lang::jlong skip(jxx::lang::jlong n) override;
        jxx::lang::jint available() override;
        void close() override;

        void readFully(const jxx::lang::ByteArray b) override;
        void readFully(const jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len) override;
        jxx::lang::jint skipBytes(jxx::lang::jint n) override;

        jxx::lang::jbool readBoolean() override;
        jxx::lang::jbyte readByte() override;
        jxx::lang::jint readUnsignedByte() override;
        jxx::lang::jshort readShort() override;
        jxx::lang::jint readUnsignedShort() override;
        jxx::lang::jchar readChar() override;
        jxx::lang::jint readInt() override;
        jxx::lang::jlong readLong() override;
        jxx::lang::jfloat readFloat() override;
        jxx::lang::jdouble readDouble() override;
        jxx::Ptr<jxx::lang::String> readLine() override;
        jxx::Ptr<jxx::lang::String> readUTF() override;

        virtual jxx::Ptr<jxx::lang::Object> readObject() override;
        virtual jxx::Ptr<jxx::lang::Object> readUnshared();

    protected:
        jxx::Ptr<InputStream> in_;
    };
}
