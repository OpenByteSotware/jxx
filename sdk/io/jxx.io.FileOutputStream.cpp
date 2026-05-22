#include "jxx.io.FileOutputStream.h"

namespace jxx::io {

FileOutputStream::FileOutputStream(jxx::Ptr<jxx::lang::String> path)
    : FileOutputStream(std::move(path), false) {}

FileOutputStream::FileOutputStream(jxx::Ptr<jxx::lang::String> path, jxx::lang::jbool append) {
    if (!path) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("path"));

    std::ios::openmode mode = std::ios::binary | std::ios::out;
    if (append) mode |= std::ios::app;
    else mode |= std::ios::trunc;

    f_.open(path->utf8(), mode);
    if (!f_) throw IOException(jxx::NEW<jxx::lang::String>("Failed to open file for write"));
}

void FileOutputStream::write(jxx::lang::jint b) {
    if (!f_) throw IOException(jxx::NEW<jxx::lang::String>("stream closed"));
    char c = (char)(b & 0xFF);
    f_.put(c);
    if (!f_) throw IOException(jxx::NEW<jxx::lang::String>("write failed"));
}

void FileOutputStream::write(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) {
    OutputStream::checkBounds_(b, off, len);
    if (!f_) throw IOException(jxx::NEW<jxx::lang::String>("stream closed"));
    f_.write(reinterpret_cast<const char*>(b->data()) + off, len);
    if (!f_) throw IOException(jxx::NEW<jxx::lang::String>("write failed"));
}

void FileOutputStream::flush() { if (f_) f_.flush(); }

void FileOutputStream::close() { if (f_.is_open()) f_.close(); }

} // namespace jxx::io
