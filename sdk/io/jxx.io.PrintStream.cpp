#include "lang/jxx.lang.String.h"
#include "jxx.io.PrintStream.h"

namespace jxx::io {

PrintStream::PrintStream(jxx::Ptr<OutputStream> out, jxx::lang::jbool autoFlush)
    : FilterOutputStream(std::move(out)), autoFlush_(autoFlush) {}

void PrintStream::writeBytes_(const std::string& bytes) {
    try {
        auto ba = jxx::NEW<ByteArray>((std::uint32_t)bytes.size());
        for (std::size_t i = 0; i < bytes.size(); ++i) (*ba)[(jxx::lang::jint)i] = (jxx::lang::jbyte)bytes[i];
        out_->write(ba, 0, (jxx::lang::jint)ba->length);
        if (autoFlush_) out_->flush();
    } catch (...) {
        setTrouble_();
    }
}

void PrintStream::flush() {
    try { out_->flush(); } catch (...) { setTrouble_(); }
}

void PrintStream::close() {
    try { out_->close(); } catch (...) { setTrouble_(); }
}

jxx::lang::jbool PrintStream::checkError() const { return trouble_; }

void PrintStream::print(jxx::Ptr<jxx::lang::String> s) {
    if (!s) s = jxx::NEW<jxx::lang::String>("null");
    writeBytes_(s->utf8());
}

void PrintStream::print(jxx::lang::jbool b) { writeBytes_(b ? "true" : "false"); }
void PrintStream::print(jxx::lang::jint i) { writeBytes_(std::to_string(i)); }
void PrintStream::print(jxx::lang::jlong l) { writeBytes_(std::to_string((long long)l)); }

void PrintStream::print(jxx::lang::jfloat f) {
    std::ostringstream oss; oss.imbue(std::locale::classic()); oss << f;
    writeBytes_(oss.str());
}

void PrintStream::print(jxx::lang::jdouble d) {
    std::ostringstream oss; oss.imbue(std::locale::classic()); oss << d;
    writeBytes_(oss.str());
}

void PrintStream::print(jxx::lang::jchar c) {
    char16_t ch = (char16_t)c;
    std::string out;
    if (ch <= 0x7F) out.push_back((char)ch);
    else if (ch <= 0x7FF) {
        out.push_back((char)(0xC0 | ((ch >> 6) & 0x1F)));
        out.push_back((char)(0x80 | (ch & 0x3F)));
    } else {
        out.push_back((char)(0xE0 | ((ch >> 12) & 0x0F)));
        out.push_back((char)(0x80 | ((ch >> 6) & 0x3F)));
        out.push_back((char)(0x80 | (ch & 0x3F)));
    }
    writeBytes_(out);
}

void PrintStream::println() { writeBytes_("
"); }
void PrintStream::println(jxx::Ptr<jxx::lang::String> s) { print(s); println(); }

} // namespace jxx::io
