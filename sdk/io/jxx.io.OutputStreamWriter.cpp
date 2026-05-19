#include "jxx.io.OutputStreamWriter.h"

#include <algorithm>

namespace jxx::io {

static inline std::string upper_ascii(std::string s) {
    for (auto& ch : s) {
        if (ch >= 'a' && ch <= 'z') ch = (char)(ch - 'a' + 'A');
    }
    return s;
}

OutputStreamWriter::OutputStreamWriter(jxx::Ptr<OutputStream> out)
    : OutputStreamWriter(std::move(out), JXX_NEW<jxx::lang::String>("UTF-8")) {}

OutputStreamWriter::OutputStreamWriter(jxx::Ptr<OutputStream> out, jxx::Ptr<jxx::lang::String> charsetName)
    : out_(std::move(out)) {
    if (!out_) throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("out"));
    enc_ = parseEncoding_(charsetName);
}

OutputStreamWriter::Enc OutputStreamWriter::parseEncoding_(jxx::Ptr<jxx::lang::String> name) {
    if (!name) return Enc::UTF8;
    std::string n = upper_ascii(name->utf8());
    n.erase(std::remove_if(n.begin(), n.end(), [](unsigned char c){ return c==' ' || c=='	' || c=='
' || c=='
'; }), n.end());

    if (n == "UTF-8" || n == "UTF8") return Enc::UTF8;
    if (n == "UTF-16BE" || n == "UTF16BE") return Enc::UTF16BE;
    if (n == "UTF-16LE" || n == "UTF16LE") return Enc::UTF16LE;
    if (n == "UTF-16" || n == "UTF16") return Enc::UTF16_BOM;

    throw jxx::lang::IllegalArgumentException(JXX_NEW<jxx::lang::String>("Unsupported charset"));
}

jxx::Ptr<jxx::lang::String> OutputStreamWriter::getEncoding() const {
    switch (enc_) {
    case Enc::UTF8: return JXX_NEW<jxx::lang::String>("UTF-8");
    case Enc::UTF16BE: return JXX_NEW<jxx::lang::String>("UTF-16BE");
    case Enc::UTF16LE: return JXX_NEW<jxx::lang::String>("UTF-16LE");
    case Enc::UTF16_BOM: return JXX_NEW<jxx::lang::String>("UTF-16");
    }
    return JXX_NEW<jxx::lang::String>("UTF-8");
}

void OutputStreamWriter::writeBomIfNeeded_() {
    if (enc_ != Enc::UTF16_BOM) return;
    if (bomWritten_) return;
    out_->write(0xFE);
    out_->write(0xFF);
    bomWritten_ = true;
}

jxx::lang::jbool OutputStreamWriter::isHigh_(jxx::lang::jint cu) { return cu >= 0xD800 && cu <= 0xDBFF; }
jxx::lang::jbool OutputStreamWriter::isLow_(jxx::lang::jint cu) { return cu >= 0xDC00 && cu <= 0xDFFF; }

jxx::lang::jint OutputStreamWriter::toCodePoint_(jxx::lang::jint hi, jxx::lang::jint lo) {
    return (((hi - 0xD800) << 10) | (lo - 0xDC00)) + 0x10000;
}

void OutputStreamWriter::writeUtf16CodeUnitBE_(jxx::lang::jint cu) {
    out_->write((cu >> 8) & 0xFF);
    out_->write(cu & 0xFF);
}

void OutputStreamWriter::writeUtf16CodeUnitLE_(jxx::lang::jint cu) {
    out_->write(cu & 0xFF);
    out_->write((cu >> 8) & 0xFF);
}

void OutputStreamWriter::writeUtf8CodePoint_(jxx::lang::jint cp) {
    if (cp < 0 || cp > 0x10FFFF) cp = 0xFFFD;
    if (cp <= 0x7F) {
        out_->write(cp);
    } else if (cp <= 0x7FF) {
        out_->write(0xC0 | ((cp >> 6) & 0x1F));
        out_->write(0x80 | (cp & 0x3F));
    } else if (cp <= 0xFFFF) {
        out_->write(0xE0 | ((cp >> 12) & 0x0F));
        out_->write(0x80 | ((cp >> 6) & 0x3F));
        out_->write(0x80 | (cp & 0x3F));
    } else {
        out_->write(0xF0 | ((cp >> 18) & 0x07));
        out_->write(0x80 | ((cp >> 12) & 0x3F));
        out_->write(0x80 | ((cp >> 6) & 0x3F));
        out_->write(0x80 | (cp & 0x3F));
    }
}

void OutputStreamWriter::write(jxx::lang::jint c) {
    writeBomIfNeeded_();
    jxx::lang::jint cu = c & 0xFFFF;

    if (enc_ == Enc::UTF16BE || enc_ == Enc::UTF16_BOM) {
        writeUtf16CodeUnitBE_(cu);
        return;
    }
    if (enc_ == Enc::UTF16LE) {
        writeUtf16CodeUnitLE_(cu);
        return;
    }

    // UTF-8: single code unit, surrogate => replacement
    if (isHigh_(cu) || isLow_(cu)) writeUtf8CodePoint_(0xFFFD);
    else writeUtf8CodePoint_(cu);
}

void OutputStreamWriter::write(jxx::Ptr<CharArray> cbuf, jxx::lang::jint off, jxx::lang::jint len) {
    Writer::checkBounds_(cbuf, off, len);
    writeBomIfNeeded_();

    if (enc_ == Enc::UTF16BE || enc_ == Enc::UTF16_BOM) {
        for (jxx::lang::jint i = 0; i < len; ++i) {
            writeUtf16CodeUnitBE_(((jxx::lang::jint)(*cbuf)[off + i]) & 0xFFFF);
        }
        return;
    }
    if (enc_ == Enc::UTF16LE) {
        for (jxx::lang::jint i = 0; i < len; ++i) {
            writeUtf16CodeUnitLE_(((jxx::lang::jint)(*cbuf)[off + i]) & 0xFFFF);
        }
        return;
    }

    for (jxx::lang::jint i = 0; i < len; ++i) {
        jxx::lang::jint cu = ((jxx::lang::jint)(*cbuf)[off + i]) & 0xFFFF;
        if (isHigh_(cu) && (i + 1) < len) {
            jxx::lang::jint lo = ((jxx::lang::jint)(*cbuf)[off + i + 1]) & 0xFFFF;
            if (isLow_(lo)) {
                writeUtf8CodePoint_(toCodePoint_(cu, lo));
                ++i;
                continue;
            }
        }
        if (isHigh_(cu) || isLow_(cu)) writeUtf8CodePoint_(0xFFFD);
        else writeUtf8CodePoint_(cu);
    }
}

void OutputStreamWriter::write(jxx::Ptr<jxx::lang::String> str, jxx::lang::jint off, jxx::lang::jint len) {
    Writer::checkStringBounds_(str, off, len);
    writeBomIfNeeded_();
    const auto& u16 = str->utf16();

    if (enc_ == Enc::UTF16BE || enc_ == Enc::UTF16_BOM) {
        for (jxx::lang::jint i = 0; i < len; ++i) {
            writeUtf16CodeUnitBE_(((jxx::lang::jint)u16[(std::size_t)(off + i)]) & 0xFFFF);
        }
        return;
    }
    if (enc_ == Enc::UTF16LE) {
        for (jxx::lang::jint i = 0; i < len; ++i) {
            writeUtf16CodeUnitLE_(((jxx::lang::jint)u16[(std::size_t)(off + i)]) & 0xFFFF);
        }
        return;
    }

    for (jxx::lang::jint i = 0; i < len; ++i) {
        jxx::lang::jint cu = ((jxx::lang::jint)u16[(std::size_t)(off + i)]) & 0xFFFF;
        if (isHigh_(cu) && (i + 1) < len) {
            jxx::lang::jint lo = ((jxx::lang::jint)u16[(std::size_t)(off + i + 1)]) & 0xFFFF;
            if (isLow_(lo)) {
                writeUtf8CodePoint_(toCodePoint_(cu, lo));
                ++i;
                continue;
            }
        }
        if (isHigh_(cu) || isLow_(cu)) writeUtf8CodePoint_(0xFFFD);
        else writeUtf8CodePoint_(cu);
    }
}

void OutputStreamWriter::flush() { out_->flush(); }
void OutputStreamWriter::close() { out_->close(); }

} // namespace jxx::io
