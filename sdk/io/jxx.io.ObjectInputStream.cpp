#include <stdexcept>
#include "lang/jxx.lang.String.h"
#include "jxx.io.DataInputStream.h"
#include "jxx.io.ObjectInputStream.h"

namespace
{
    [[noreturn]] void throwUnsupported_(const char* msg)
    {
        throw std::runtime_error(msg);
    }
}

namespace jxx::io
{
    ObjectInputStream::ObjectInputStream(jxx::Ptr<InputStream> in)
        : in_(std::move(in))
    {
        if (!in_)
            throwUnsupported_("null underlying stream");
    }

    jxx::lang::jint ObjectInputStream::read() { return in_->read(); }
    jxx::lang::jint ObjectInputStream::read(const jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len) { return in_->read(b, off, len); }
    jxx::lang::jlong ObjectInputStream::skip(jxx::lang::jlong n) { return in_->skip(n); }
    jxx::lang::jint ObjectInputStream::available() { return in_->available(); }
    void ObjectInputStream::close() { in_->close(); }

    void ObjectInputStream::readFully(const jxx::lang::ByteArray b)
    {
        auto din = jxx::NEW<DataInputStream>(in_);
        din->readFully(b);
    }

    void ObjectInputStream::readFully(const jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len)
    {
        auto din = jxx::NEW<DataInputStream>(in_);
        din->readFully(b, off, len);
    }

    jxx::lang::jint ObjectInputStream::skipBytes(jxx::lang::jint n)
    {
        auto din = jxx::NEW<DataInputStream>(in_);
        return din->skipBytes(n);
    }

    jxx::lang::jbool ObjectInputStream::readBoolean() { return jxx::NEW<DataInputStream>(in_)->readBoolean(); }
    jxx::lang::jbyte ObjectInputStream::readByte() { return jxx::NEW<DataInputStream>(in_)->readByte(); }
    jxx::lang::jint ObjectInputStream::readUnsignedByte() { return jxx::NEW<DataInputStream>(in_)->readUnsignedByte(); }
    jxx::lang::jshort ObjectInputStream::readShort() { return jxx::NEW<DataInputStream>(in_)->readShort(); }
    jxx::lang::jint ObjectInputStream::readUnsignedShort() { return jxx::NEW<DataInputStream>(in_)->readUnsignedShort(); }
    jxx::lang::jchar ObjectInputStream::readChar() { return jxx::NEW<DataInputStream>(in_)->readChar(); }
    jxx::lang::jint ObjectInputStream::readInt() { return jxx::NEW<DataInputStream>(in_)->readInt(); }
    jxx::lang::jlong ObjectInputStream::readLong() { return jxx::NEW<DataInputStream>(in_)->readLong(); }
    jxx::lang::jfloat ObjectInputStream::readFloat() { return jxx::NEW<DataInputStream>(in_)->readFloat(); }
    jxx::lang::jdouble ObjectInputStream::readDouble() { return jxx::NEW<DataInputStream>(in_)->readDouble(); }
    jxx::Ptr<jxx::lang::String> ObjectInputStream::readLine() { return jxx::NEW<DataInputStream>(in_)->readLine(); }
    jxx::Ptr<jxx::lang::String> ObjectInputStream::readUTF() { return jxx::NEW<DataInputStream>(in_)->readUTF(); }

    jxx::Ptr<jxx::lang::Object> ObjectInputStream::readObject()
    {
        throwUnsupported_("ObjectInputStream::readObject requires serialization framework");
    }

    jxx::Ptr<jxx::lang::Object> ObjectInputStream::readUnshared()
    {
        throwUnsupported_("ObjectInputStream::readUnshared requires serialization framework");
    }
}
