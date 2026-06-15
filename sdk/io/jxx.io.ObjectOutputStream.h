#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "jxx.io.OutputStream.h"
#include "jxx.io.ObjectOutput.h"

namespace jxx::io
{
    class ObjectOutputStream : public OutputStream, public ObjectOutput
    {
    public:
        explicit ObjectOutputStream(jxx::Ptr<OutputStream> out);
        ~ObjectOutputStream() override = default;

    public:
        void write(jxx::lang::jint b) override;
        void write(const jxx::lang::ByteArray b,
            jxx::lang::jint off,
            jxx::lang::jint len) override;
        void flush() override;
        void close() override;

        void writeBoolean(jxx::lang::jbool v) override;
        void writeByte(jxx::lang::jint v) override;
        void writeShort(jxx::lang::jint v) override;
        void writeChar(jxx::lang::jint v) override;
        void writeInt(jxx::lang::jint v) override;
        void writeLong(jxx::lang::jlong v) override;
        void writeFloat(jxx::lang::jfloat v) override;
        void writeDouble(jxx::lang::jdouble v) override;
        void writeBytes(jxx::Ptr<jxx::lang::String> s) override;
        void writeChars(jxx::Ptr<jxx::lang::String> s) override;
        void writeUTF(jxx::Ptr<jxx::lang::String> s) override;

        virtual void writeObject(jxx::Ptr<jxx::lang::Object> obj);
        virtual void writeUnshared(jxx::Ptr<jxx::lang::Object> obj);

    protected:
        jxx::Ptr<OutputStream> out_;
    };
}