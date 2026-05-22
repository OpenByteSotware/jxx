#include "jxx.io.DataOutputStream.h"

#include <cstring>

namespace jxx::io {

DataOutputStream::DataOutputStream(jxx::Ptr<OutputStream> out)
    : FilterOutputStream(std::move(out)) {}

jxx::lang::jint DataOutputStream::size() const { return written_; }

void DataOutputStream::write(jxx::lang::jint b) {
    out_->write(b);
    inc_(1);
}

void DataOutputStream::write(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) {
    out_->write(b, off, len);
    inc_(len);
}

void DataOutputStream::writeBoolean(jxx::lang::jbool v) { write(v ? 1 : 0); }
void DataOutputStream::writeByte(jxx::lang::jint v) { write(v); }

void DataOutputStream::writeShort(jxx::lang::jint v) {
    write((v >> 8) & 0xFF);
    write(v & 0xFF);
}

void DataOutputStream::writeChar(jxx::lang::jint v) { writeShort(v); }

void DataOutputStream::writeInt(jxx::lang::jint v) {
    write((v >> 24) & 0xFF);
    write((v >> 16) & 0xFF);
    write((v >> 8) & 0xFF);
    write(v & 0xFF);
}

void DataOutputStream::writeLong(jxx::lang::jlong v) {
    writeInt((jxx::lang::jint)((v >> 32) & 0xFFFFFFFFLL));
    writeInt((jxx::lang::jint)(v & 0xFFFFFFFFLL));
}

void DataOutputStream::writeFloat(jxx::lang::jfloat v) {
    jxx::lang::jint i;
    static_assert(sizeof(i) == sizeof(v), "float size mismatch");
    std::memcpy(&i, &v, sizeof(i));
    writeInt(i);
}

void DataOutputStream::writeDouble(jxx::lang::jdouble v) {
    jxx::lang::jlong l;
    static_assert(sizeof(l) == sizeof(v), "double size mismatch");
    std::memcpy(&l, &v, sizeof(l));
    writeLong(l);
}

void DataOutputStream::writeBytes(jxx::Ptr<jxx::lang::String> s) {
    if (!s) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("s"));
    const auto& u16 = s->utf16();
    for (char16_t ch : u16) {
        write(((jxx::lang::jint)ch) & 0xFF);
    }
}

void DataOutputStream::writeChars(jxx::Ptr<jxx::lang::String> s) {
    if (!s) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("s"));
    const auto& u16 = s->utf16();
    for (char16_t ch : u16) {
        write((((jxx::lang::jint)ch) >> 8) & 0xFF);
        write(((jxx::lang::jint)ch) & 0xFF);
    }
}

static void appendModifiedUtf8(std::string& out, char16_t ch) {
    if (ch == 0x0000) {
        out.push_back((char)0xC0);
        out.push_back((char)0x80);
    } else if (ch <= 0x007F) {
        out.push_back((char)ch);
    } else if (ch <= 0x07FF) {
        out.push_back((char)(0xC0 | ((ch >> 6) & 0x1F)));
        out.push_back((char)(0x80 | (ch & 0x3F)));
    } else {
        out.push_back((char)(0xE0 | ((ch >> 12) & 0x0F)));
        out.push_back((char)(0x80 | ((ch >> 6) & 0x3F)));
        out.push_back((char)(0x80 | (ch & 0x3F)));
    }
}

void DataOutputStream::writeUTF(jxx::Ptr<jxx::lang::String> s) {
    if (!s) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("s"));
    const auto& u16 = s->utf16();

    std::string enc;
    enc.reserve(u16.size() * 3);
    for (char16_t ch : u16) appendModifiedUtf8(enc, ch);

    if (enc.size() > 65535) {
        throw IOException(jxx::NEW<jxx::lang::String>("UTF string too long"));
    }

    // unsigned short length
    writeShort((jxx::lang::jint)enc.size());

    auto ba = jxx::NEW<jxx::lang::ByteArrayType>((std::uint32_t)enc.size());
    for (std::size_t i = 0; i < enc.size(); ++i) (*ba)[(jxx::lang::jint)i] = (jxx::lang::jbyte)enc[i];

    write(ba, 0, (jxx::lang::jint)ba->length);
}

} // namespace jxx::io
