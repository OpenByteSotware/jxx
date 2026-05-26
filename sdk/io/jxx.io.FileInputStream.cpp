#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "jxx.io.IOException.h"
#include "jxx.io.FileInputStream.h"


namespace jxx::io {

FileInputStream::FileInputStream(jxx::Ptr<jxx::lang::String> path) {
    if (!path) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("path"));
    f_.open(path->utf8(), std::ios::binary);
    if (!f_) throw IOException(jxx::NEW<jxx::lang::String>("Failed to open file"));
}

jxx::lang::jint FileInputStream::read() {
    if (!f_) return -1;
    int c = f_.get();
    if (c == EOF) return -1;
    return c & 0xFF;
}

jxx::lang::jint FileInputStream::read(jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len) {
    InputStream::checkBounds_(b, off, len);
    if (len == 0) return 0;
    if (!f_) return -1;

    f_.read(reinterpret_cast<char*>(b->data()) + off, len);
    std::streamsize got = f_.gcount();
    if (got <= 0) return -1;
    return (jxx::lang::jint)got;
}

jxx::lang::jint FileInputStream::available() {
    if (!f_) return 0;
    auto cur = f_.tellg();
    if (cur == std::streampos(-1)) return 0;
    auto end = f_.seekg(0, std::ios::end).tellg();
    f_.seekg(cur);
    if (end == std::streampos(-1)) return 0;
    auto rem = end - cur;
    if (rem < 0) return 0;
    if (rem > std::streamoff(0x7fffffff)) return 0x7fffffff;
    return (jxx::lang::jint)rem;
}

void FileInputStream::close() {
    if (f_.is_open()) f_.close();
}

} // namespace jxx::io
