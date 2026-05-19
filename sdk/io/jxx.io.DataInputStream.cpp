#include "jxx.io.DataInputStream.h"
#include "jxx.lang.StringBuilder.h"

#include <cstring>

namespace jxx::io {

DataInputStream::DataInputStream(jxx::Ptr<InputStream> in)
    : FilterInputStream(std::move(in)) {}

void DataInputStream::readFullyRaw_(jxx::Ptr<ByteArray> b, jint off, jint len) {
    jint total = 0;
    while (total < len) {
        jint r = in_->read(b, off + total, len - total);
        if (r < 0) throw EOFException(JXX_NEW<jxx::lang::String>("EOF"));
        total += r;
    }
}

void DataInputStream::readFully(jxx::Ptr<ByteArray> b) {
    if (!b) throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("b"));
    readFullyRaw_(b, 0, (jint)b->length);
}

void DataInputStream::readFully(jxx::Ptr<ByteArray> b, jint off, jint len) {
    InputStream::checkBounds_(b, off, len);
    readFullyRaw_(b, off, len);
}

jint DataInputStream::skipBytes(jint n) {
    if (n <= 0) return 0;
    return (jint)in_->skip(n);
}

jbool DataInputStream::readBoolean() {
    jint c = in_->read();
    if (c < 0) throw EOFException(JXX_NEW<jxx::lang::String>("EOF"));
    return c != 0;
}

jbyte DataInputStream::readByte() {
    jint c = in_->read();
    if (c < 0) throw EOFException(JXX_NEW<jxx::lang::String>("EOF"));
    return (jbyte)c;
}

jint DataInputStream::readUnsignedByte() {
    jint c = in_->read();
    if (c < 0) throw EOFException(JXX_NEW<jxx::lang::String>("EOF"));
    return c & 0xFF;
}

jshort DataInputStream::readShort() {
    jint hi = readUnsignedByte();
    jint lo = readUnsignedByte();
    return (jshort)((hi << 8) | lo);
}

jint DataInputStream::readUnsignedShortBE_() {
    jint hi = readUnsignedByte();
    jint lo = readUnsignedByte();
    return ((hi << 8) | lo) & 0xFFFF;
}

jint DataInputStream::readUnsignedShort() {
    return readUnsignedShortBE_();
}

jchar DataInputStream::readChar() {
    return (jchar)readUnsignedShortBE_();
}

jint DataInputStream::readInt() {
    jint b1 = readUnsignedByte();
    jint b2 = readUnsignedByte();
    jint b3 = readUnsignedByte();
    jint b4 = readUnsignedByte();
    return (jint)((b1 << 24) | (b2 << 16) | (b3 << 8) | b4);
}

jlong DataInputStream::readLong() {
    jlong hi = (jlong)readInt() & 0xFFFFFFFFLL;
    jlong lo = (jlong)readInt() & 0xFFFFFFFFLL;
    return (hi << 32) | lo;
}

jfloat DataInputStream::readFloat() {
    jint i = readInt();
    jfloat f;
    static_assert(sizeof(jfloat) == sizeof(jint), "float size mismatch");
    std::memcpy(&f, &i, sizeof(f));
    return f;
}

jdouble DataInputStream::readDouble() {
    jlong l = readLong();
    jdouble d;
    static_assert(sizeof(jdouble) == sizeof(jlong), "double size mismatch");
    std::memcpy(&d, &l, sizeof(d));
    return d;
}

jxx::Ptr<jxx::lang::String> DataInputStream::readLine() {
    auto sb = JXX_NEW<jxx::lang::StringBuilder>();
    jint c = in_->read();
    if (c < 0) return nullptr;

    while (c >= 0) {
        if (c == '
') break;
        if (c == '
') {
            jint n = in_->read();
            (void)n;
            break;
        }
        sb->append((jchar)c);
        c = in_->read();
    }
    return sb->toString();
}

jxx::Ptr<jxx::lang::String> DataInputStream::readUTF() {
    jint utflen = readUnsignedShortBE_();
    auto data = JXX_NEW<ByteArray>((std::uint32_t)utflen);
    readFullyRaw_(data, 0, utflen);

    std::u16string out;
    out.reserve((std::size_t)utflen);

    jint i = 0;
    while (i < utflen) {
        jint a = ((jint)(*data)[i]) & 0xFF;

        if (a < 0x80) {
            out.push_back((char16_t)a);
            ++i;
        } else if ((a & 0xE0) == 0xC0) {
            if (i + 1 >= utflen) throw IOException(JXX_NEW<jxx::lang::String>("malformed UTF"));
            jint b = ((jint)(*data)[i + 1]) & 0x3F;
            jint ch = ((a & 0x1F) << 6) | b;
            out.push_back((char16_t)ch);
            i += 2;
        } else if ((a & 0xF0) == 0xE0) {
            if (i + 2 >= utflen) throw IOException(JXX_NEW<jxx::lang::String>("malformed UTF"));
            jint b = ((jint)(*data)[i + 1]) & 0x3F;
            jint c = ((jint)(*data)[i + 2]) & 0x3F;
            jint ch = ((a & 0x0F) << 12) | (b << 6) | c;
            out.push_back((char16_t)ch);
            i += 3;
        } else {
            throw IOException(JXX_NEW<jxx::lang::String>("malformed UTF"));
        }
    }

    auto ca = JXX_NEW<CharArray>((std::uint32_t)out.size());
    for (jint k = 0; k < (jint)out.size(); ++k) (*ca)[k] = (jchar)out[(std::size_t)k];
    return JXX_NEW<jxx::lang::String>(ca);
}

} // namespace jxx::io
