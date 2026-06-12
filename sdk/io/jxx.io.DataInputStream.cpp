#include <cstring>
#include "jxx.io.EOFException.h"
#include "jxx.io.IOException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StringBuilder.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "jxx.io.DataInputStream.h"

namespace jxx::io {

DataInputStream::DataInputStream(jxx::Ptr<InputStream> in)
    : FilterInputStream(std::move(in)) {}

void DataInputStream::readFullyRaw_(jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len) {
    jxx::lang::jint total = 0;
    while (total < len) {
        jxx::lang::jint r = in_->read(b, off + total, len - total);
        if (r < 0) throw EOFException(jxx::NEW<jxx::lang::String>("EOF"));
        total += r;
    }
}

void DataInputStream::readFully(jxx::lang::ByteArray b) {
    if (!b) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("b"));
    readFullyRaw_(b, 0, (jxx::lang::jint)b->length);
}

void DataInputStream::readFully(jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len) {
    InputStream::checkBounds_(b, off, len);
    readFullyRaw_(b, off, len);
}

jxx::lang::jint DataInputStream::skipBytes(jxx::lang::jint n) {
    if (n <= 0) return 0;
    return (jxx::lang::jint)in_->skip(n);
}

jxx::lang::jbool DataInputStream::readBoolean() {
    jxx::lang::jint c = in_->read();
    if (c < 0) throw EOFException(jxx::NEW<jxx::lang::String>("EOF"));
    return c != 0;
}

jxx::lang::jbyte DataInputStream::readByte() {
    jxx::lang::jint c = in_->read();
    if (c < 0) throw EOFException(jxx::NEW<jxx::lang::String>("EOF"));
    return (jxx::lang::jbyte)c;
}

jxx::lang::jint DataInputStream::readUnsignedByte() {
    jxx::lang::jint c = in_->read();
    if (c < 0) throw EOFException(jxx::NEW<jxx::lang::String>("EOF"));
    return c & 0xFF;
}

jxx::lang::jshort DataInputStream::readShort() {
    jxx::lang::jint hi = readUnsignedByte();
    jxx::lang::jint lo = readUnsignedByte();
    return (jxx::lang::jshort)((hi << 8) | lo);
}

jxx::lang::jint DataInputStream::readUnsignedShortBE_() {
    jxx::lang::jint hi = readUnsignedByte();
    jxx::lang::jint lo = readUnsignedByte();
    return ((hi << 8) | lo) & 0xFFFF;
}

jxx::lang::jint DataInputStream::readUnsignedShort() {
    return readUnsignedShortBE_();
}

jxx::lang::jchar DataInputStream::readChar() {
    return (jxx::lang::jchar)readUnsignedShortBE_();
}

jxx::lang::jint DataInputStream::readInt() {
    jxx::lang::jint b1 = readUnsignedByte();
    jxx::lang::jint b2 = readUnsignedByte();
    jxx::lang::jint b3 = readUnsignedByte();
    jxx::lang::jint b4 = readUnsignedByte();
    return (jxx::lang::jint)((b1 << 24) | (b2 << 16) | (b3 << 8) | b4);
}

jxx::lang::jlong DataInputStream::readLong() {
    jxx::lang::jlong hi = (jxx::lang::jlong)readInt() & 0xFFFFFFFFLL;
    jxx::lang::jlong lo = (jxx::lang::jlong)readInt() & 0xFFFFFFFFLL;
    return (hi << 32) | lo;
}

jxx::lang::jfloat DataInputStream::readFloat() {
    jxx::lang::jint i = readInt();
    jxx::lang::jfloat f;
    static_assert(sizeof(jxx::lang::jfloat) == sizeof(jxx::lang::jint), "float size mismatch");
    std::memcpy(&f, &i, sizeof(f));
    return f;
}

jxx::lang::jdouble DataInputStream::readDouble() {
    jxx::lang::jlong l = readLong();
    jxx::lang::jdouble d;
    static_assert(sizeof(jxx::lang::jdouble) == sizeof(jxx::lang::jlong), "double size mismatch");
    std::memcpy(&d, &l, sizeof(d));
    return d;
}

jxx::Ptr<jxx::lang::String> DataInputStream::readLine() {
    auto sb = jxx::NEW<jxx::lang::StringBuilder>();
    jxx::lang::jint c = in_->read();
    if (c < 0) return nullptr;

    while (c >= 0) {
        if (c == '\n') break;
        if (c == '\r') {
            jxx::lang::jint n = in_->read();
            (void)n;
            break;
        }
        sb->append((jxx::lang::jchar)c);
        c = in_->read();
    }
    return sb->toString();
}

jxx::Ptr<jxx::lang::String> DataInputStream::readUTF() {
    jxx::lang::jint utflen = readUnsignedShortBE_();
    auto data = jxx::NEW<jxx::lang::ByteArrayType>((std::uint32_t)utflen);
    readFullyRaw_(data, 0, utflen);

    std::u16string out;
    out.reserve((std::size_t)utflen);

    jxx::lang::jint i = 0;
    while (i < utflen) {
        jxx::lang::jint a = ((jxx::lang::jint)(*data)[i]) & 0xFF;

        if (a < 0x80) {
            out.push_back((char16_t)a);
            ++i;
        } else if ((a & 0xE0) == 0xC0) {
            if (i + 1 >= utflen) throw IOException(jxx::NEW<jxx::lang::String>("malformed UTF"));
            jxx::lang::jint b = ((jxx::lang::jint)(*data)[i + 1]) & 0x3F;
            jxx::lang::jint ch = ((a & 0x1F) << 6) | b;
            out.push_back((char16_t)ch);
            i += 2;
        } else if ((a & 0xF0) == 0xE0) {
            if (i + 2 >= utflen) throw IOException(jxx::NEW<jxx::lang::String>("malformed UTF"));
            jxx::lang::jint b = ((jxx::lang::jint)(*data)[i + 1]) & 0x3F;
            jxx::lang::jint c = ((jxx::lang::jint)(*data)[i + 2]) & 0x3F;
            jxx::lang::jint ch = ((a & 0x0F) << 12) | (b << 6) | c;
            out.push_back((char16_t)ch);
            i += 3;
        } else {
            throw IOException(jxx::NEW<jxx::lang::String>("malformed UTF"));
        }
    }

    auto ca = jxx::NEW<jxx::lang::CharArrayType>((std::uint32_t)out.size());
    for (jxx::lang::jint k = 0; k < (jxx::lang::jint)out.size(); ++k) (*ca)[k] = (jxx::lang::jchar)out[(std::size_t)k];
    return jxx::NEW<jxx::lang::String>(ca);
}

} // namespace jxx::io
