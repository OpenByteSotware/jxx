#include "io/jxx.io.RandomAccessFile.h"

#include <algorithm>
#include <cerrno>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <limits>
#include <string>
#include <vector>

#if defined(_WIN32)
#include <io.h>
#else
#include <unistd.h>
#endif

#include "io/jxx.io.EOFException.h"
#include "io/jxx.io.File.h"
#include "io/jxx.io.FileDescriptor.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "io/jxx.io.IOException.h"
#include "io/jxx.io.UTFDataFormatException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "nio/channels/jxx.nio.channels.FileChannel.h"

namespace {

#if defined(_WIN32)
auto seek64(FILE* handle, std::int64_t offset, int origin) -> int {
    return _fseeki64(handle, offset, origin);
}
auto tell64(FILE* handle) -> std::int64_t {
    return _ftelli64(handle);
}
#else
auto seek64(FILE* handle, std::int64_t offset, int origin) -> int {
    return fseeko(handle, static_cast<off_t>(offset), origin);
}
auto tell64(FILE* handle) -> std::int64_t {
    return static_cast<std::int64_t>(ftello(handle));
}
#endif

std::vector<unsigned char> encodeModifiedUtf8(const std::u16string& text) {
    std::vector<unsigned char> result;
    for (const auto character : text) {
        const auto value = static_cast<std::uint16_t>(character);
        if (value >= 0x0001U && value <= 0x007FU) {
            result.push_back(static_cast<unsigned char>(value));
        } else if (value <= 0x07FFU) {
            result.push_back(static_cast<unsigned char>(0xC0U | (value >> 6U)));
            result.push_back(static_cast<unsigned char>(0x80U | (value & 0x3FU)));
        } else {
            result.push_back(static_cast<unsigned char>(0xE0U | (value >> 12U)));
            result.push_back(static_cast<unsigned char>(0x80U | ((value >> 6U) & 0x3FU)));
            result.push_back(static_cast<unsigned char>(0x80U | (value & 0x3FU)));
        }
    }
    return result;
}

} // namespace

namespace jxx::io {

RandomAccessFile::RandomAccessFile(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& mode)
    : Super() {
    if (name == nullptr) throw ::jxx::lang::NullPointerException();
    open_(::jxx::NEW<File>(name), mode);
}

RandomAccessFile::RandomAccessFile(
    const ::jxx::Ptr<File>& file,
    const ::jxx::Ptr<::jxx::lang::String>& mode)
    : Super() {
    open_(file, mode);
}

RandomAccessFile::~RandomAccessFile() {
    try { close(); } catch (...) {}
}

void RandomAccessFile::open_(
    const ::jxx::Ptr<File>& file,
    const ::jxx::Ptr<::jxx::lang::String>& mode) {
    if (file == nullptr || mode == nullptr)
        throw ::jxx::lang::NullPointerException();
    const auto modeText = mode->utf8();
    if (modeText != "r" && modeText != "rw" &&
        modeText != "rws" && modeText != "rwd")
        throw ::jxx::lang::IllegalArgumentException();

    file_ = file;
    writable_ = modeText != "r";
    syncData_ = modeText == "rws" || modeText == "rwd";
    syncMetadata_ = modeText == "rws";
    const auto path = file->getPath()->utf8();
    handle_ = std::fopen(path.c_str(), writable_ ? "r+b" : "rb");
    if (handle_ == nullptr && writable_)
        handle_ = std::fopen(path.c_str(), "w+b");
    if (handle_ == nullptr)
        throw FileNotFoundException(path);
    descriptor_ = ::jxx::NEW<FileDescriptor>(handle_, false);
}

void RandomAccessFile::ensureOpen_() const {
    if (handle_ == nullptr) throw IOException("Stream Closed");
}

void RandomAccessFile::ensureWritable_() const {
    ensureOpen_();
    if (!writable_) throw IOException("File opened read-only");
}

void RandomAccessFile::validateRange_(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint count) const {
    if (buffer == nullptr) throw ::jxx::lang::NullPointerException();
    if (offset < 0 || count < 0 ||
        offset > static_cast<::jxx::lang::jint>(buffer->size()) - count)
        throw ::jxx::lang::IndexOutOfBoundsException();
}

::jxx::Ptr<FileDescriptor> RandomAccessFile::getFD() {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    ensureOpen_();
    return descriptor_;
}

::jxx::Ptr<::jxx::nio::channels::FileChannel> RandomAccessFile::getChannel() {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    ensureOpen_();
    if (channel_ == nullptr)
        channel_ = ::jxx::nio::channels::FileChannel::open(
            file_, true, writable_, false);
    return channel_;
}

::jxx::lang::jint RandomAccessFile::read() {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    ensureOpen_();
    const int value = std::fgetc(handle_);
    if (value == EOF) {
        if (std::feof(handle_)) return -1;
        throw IOException();
    }
    return static_cast<::jxx::lang::jint>(static_cast<unsigned char>(value));
}

::jxx::lang::jint RandomAccessFile::read(const ::jxx::lang::ByteArray& buffer) {
    if (buffer == nullptr) throw ::jxx::lang::NullPointerException();
    return read(buffer, 0, static_cast<::jxx::lang::jint>(buffer->size()));
}

::jxx::lang::jint RandomAccessFile::read(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint count) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    ensureOpen_();
    validateRange_(buffer, offset, count);
    if (count == 0) return 0;
    std::vector<unsigned char> temporary(static_cast<std::size_t>(count));
    const auto actual = std::fread(temporary.data(), 1, temporary.size(), handle_);
    if (actual == 0) {
        if (std::feof(handle_)) return -1;
        throw IOException();
    }
    for (std::size_t index = 0; index < actual; ++index)
        (*buffer)[offset + static_cast<::jxx::lang::jint>(index)] =
            static_cast<::jxx::lang::jbyte>(temporary[index]);
    return static_cast<::jxx::lang::jint>(actual);
}

void RandomAccessFile::readFully(const ::jxx::lang::ByteArray& buffer) {
    if (buffer == nullptr) throw ::jxx::lang::NullPointerException();
    readFully(buffer, 0, static_cast<::jxx::lang::jint>(buffer->size()));
}

void RandomAccessFile::readFully(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint count) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    validateRange_(buffer, offset, count);
    ::jxx::lang::jint total = 0;
    while (total < count) {
        const auto actual = read(buffer, offset + total, count - total);
        if (actual < 0) throw EOFException();
        total += actual;
    }
}

::jxx::lang::jint RandomAccessFile::skipBytes(::jxx::lang::jint count) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    ensureOpen_();
    if (count <= 0) return 0;
    const auto current = getFilePointer();
    const auto remaining = length() - current;
    const auto skipped = std::min<::jxx::lang::jlong>(count, std::max<::jxx::lang::jlong>(0, remaining));
    seek(current + skipped);
    return static_cast<::jxx::lang::jint>(skipped);
}

::jxx::lang::jint RandomAccessFile::readRequiredByte_() {
    const auto value = read();
    if (value < 0) throw EOFException();
    return value;
}

::jxx::lang::jbool RandomAccessFile::readBoolean() { return readRequiredByte_() != 0; }
::jxx::lang::jbyte RandomAccessFile::readByte() { return static_cast<::jxx::lang::jbyte>(readRequiredByte_()); }
::jxx::lang::jint RandomAccessFile::readUnsignedByte() { return readRequiredByte_(); }
::jxx::lang::jshort RandomAccessFile::readShort() { return static_cast<::jxx::lang::jshort>(readUnsignedShort()); }
::jxx::lang::jint RandomAccessFile::readUnsignedShort() {
    return (readRequiredByte_() << 8) | readRequiredByte_();
}
::jxx::lang::jchar RandomAccessFile::readChar() { return static_cast<::jxx::lang::jchar>(readUnsignedShort()); }
::jxx::lang::jint RandomAccessFile::readInt() {
    const auto b1 = readRequiredByte_(); const auto b2 = readRequiredByte_();
    const auto b3 = readRequiredByte_(); const auto b4 = readRequiredByte_();
    const auto value = (static_cast<std::uint32_t>(b1) << 24U) |
        (static_cast<std::uint32_t>(b2) << 16U) |
        (static_cast<std::uint32_t>(b3) << 8U) | static_cast<std::uint32_t>(b4);
    return static_cast<::jxx::lang::jint>(value);
}
::jxx::lang::jlong RandomAccessFile::readLong() {
    const auto high = static_cast<std::uint32_t>(readInt());
    const auto low = static_cast<std::uint32_t>(readInt());
    return static_cast<::jxx::lang::jlong>((static_cast<std::uint64_t>(high) << 32U) | low);
}
::jxx::lang::jfloat RandomAccessFile::readFloat() {
    const auto bits = static_cast<std::uint32_t>(readInt());
    ::jxx::lang::jfloat value; std::memcpy(&value, &bits, sizeof(value)); return value;
}
::jxx::lang::jdouble RandomAccessFile::readDouble() {
    const auto bits = static_cast<std::uint64_t>(readLong());
    ::jxx::lang::jdouble value; std::memcpy(&value, &bits, sizeof(value)); return value;
}

::jxx::Ptr<::jxx::lang::String> RandomAccessFile::readLine() {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    ensureOpen_();
    std::string result;
    bool sawAny = false;
    for (;;) {
        const auto value = read();
        if (value < 0) break;
        sawAny = true;
        if (value == '\n') break;
        if (value == '\r') {
            const auto position = getFilePointer();
            const auto next = read();
            if (next != '\n' && next >= 0) seek(position);
            break;
        }
        result.push_back(static_cast<char>(value));
    }
    if (!sawAny && result.empty()) return nullptr;
    return ::jxx::NEW<::jxx::lang::String>(result);
}

::jxx::Ptr<::jxx::lang::String> RandomAccessFile::readUTF() {
    const auto count = readUnsignedShort();
    std::vector<unsigned char> bytes(static_cast<std::size_t>(count));
    for (auto& value : bytes) value = static_cast<unsigned char>(readRequiredByte_());
    std::u16string result;
    for (std::size_t index = 0; index < bytes.size();) {
        const auto first = bytes[index++];
        if ((first & 0x80U) == 0U) {
            if (first == 0U) throw UTFDataFormatException();
            result.push_back(static_cast<char16_t>(first));
        } else if ((first & 0xE0U) == 0xC0U) {
            if (index >= bytes.size()) throw UTFDataFormatException();
            const auto second = bytes[index++];
            if ((second & 0xC0U) != 0x80U) throw UTFDataFormatException();
            result.push_back(static_cast<char16_t>(((first & 0x1FU) << 6U) | (second & 0x3FU)));
        } else if ((first & 0xF0U) == 0xE0U) {
            if (index + 1 >= bytes.size()) throw UTFDataFormatException();
            const auto second = bytes[index++]; const auto third = bytes[index++];
            if ((second & 0xC0U) != 0x80U || (third & 0xC0U) != 0x80U)
                throw UTFDataFormatException();
            result.push_back(static_cast<char16_t>(((first & 0x0FU) << 12U) |
                ((second & 0x3FU) << 6U) | (third & 0x3FU)));
        } else throw UTFDataFormatException();
    }
    return ::jxx::NEW<::jxx::lang::String>(result);
}

void RandomAccessFile::syncIfRequired_() {
    if (!syncData_) return;
    if (std::fflush(handle_) != 0) throw IOException();
#if defined(_WIN32)
    if (_commit(_fileno(handle_)) != 0) throw IOException();
#else
    if (::fsync(fileno(handle_)) != 0) throw IOException();
#endif
}

void RandomAccessFile::write(::jxx::lang::jint value) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    ensureWritable_();
    if (std::fputc(value & 0xFF, handle_) == EOF) throw IOException();
    syncIfRequired_();
}

void RandomAccessFile::write(const ::jxx::lang::ByteArray& buffer) {
    if (buffer == nullptr) throw ::jxx::lang::NullPointerException();
    write(buffer, 0, static_cast<::jxx::lang::jint>(buffer->size()));
}

void RandomAccessFile::write(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint count) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    ensureWritable_(); validateRange_(buffer, offset, count);
    std::vector<unsigned char> temporary(static_cast<std::size_t>(count));
    for (std::size_t index = 0; index < temporary.size(); ++index)
        temporary[index] = static_cast<unsigned char>((*buffer)[offset + static_cast<::jxx::lang::jint>(index)]);
    if (count > 0 && std::fwrite(temporary.data(), 1, temporary.size(), handle_) != temporary.size())
        throw IOException();
    syncIfRequired_();
}

void RandomAccessFile::writeBoolean(::jxx::lang::jbool value) { write(value ? 1 : 0); }
void RandomAccessFile::writeByte(::jxx::lang::jint value) { write(value); }
void RandomAccessFile::writeShort(::jxx::lang::jint value) { write(value >> 8); write(value); }
void RandomAccessFile::writeChar(::jxx::lang::jint value) { writeShort(value); }
void RandomAccessFile::writeInt(::jxx::lang::jint value) {
    write(value >> 24); write(value >> 16); write(value >> 8); write(value);
}
void RandomAccessFile::writeLong(::jxx::lang::jlong value) {
    writeInt(static_cast<::jxx::lang::jint>(value >> 32));
    writeInt(static_cast<::jxx::lang::jint>(value));
}
void RandomAccessFile::writeFloat(::jxx::lang::jfloat value) {
    std::uint32_t bits; std::memcpy(&bits, &value, sizeof(bits));
    writeInt(static_cast<::jxx::lang::jint>(bits));
}
void RandomAccessFile::writeDouble(::jxx::lang::jdouble value) {
    std::uint64_t bits; std::memcpy(&bits, &value, sizeof(bits));
    writeLong(static_cast<::jxx::lang::jlong>(bits));
}
void RandomAccessFile::writeBytes(const ::jxx::Ptr<::jxx::lang::String>& value) {
    if (value == nullptr) throw ::jxx::lang::NullPointerException();
    for (const auto character : value->utf16()) write(static_cast<::jxx::lang::jint>(character));
}
void RandomAccessFile::writeChars(const ::jxx::Ptr<::jxx::lang::String>& value) {
    if (value == nullptr) throw ::jxx::lang::NullPointerException();
    for (const auto character : value->utf16()) writeChar(static_cast<::jxx::lang::jint>(character));
}
void RandomAccessFile::writeUTF(const ::jxx::Ptr<::jxx::lang::String>& value) {
    if (value == nullptr) throw ::jxx::lang::NullPointerException();
    const auto bytes = encodeModifiedUtf8(value->utf16());
    if (bytes.size() > 65535U) throw UTFDataFormatException();
    writeShort(static_cast<::jxx::lang::jint>(bytes.size()));
    for (const auto byte : bytes) write(static_cast<::jxx::lang::jint>(byte));
}

::jxx::lang::jlong RandomAccessFile::getFilePointer() {
    std::lock_guard<std::recursive_mutex> lock(mutex_); ensureOpen_();
    const auto position = tell64(handle_);
    if (position < 0) throw IOException();
    return static_cast<::jxx::lang::jlong>(position);
}
void RandomAccessFile::seek(::jxx::lang::jlong position) {
    std::lock_guard<std::recursive_mutex> lock(mutex_); ensureOpen_();
    if (position < 0) throw IOException("Negative seek offset");
    if (seek64(handle_, position, SEEK_SET) != 0) throw IOException();
    std::clearerr(handle_);
}
::jxx::lang::jlong RandomAccessFile::length() {
    std::lock_guard<std::recursive_mutex> lock(mutex_); ensureOpen_();
    const auto current = getFilePointer();
    if (seek64(handle_, 0, SEEK_END) != 0) throw IOException();
    const auto result = tell64(handle_);
    if (seek64(handle_, current, SEEK_SET) != 0 || result < 0) throw IOException();
    return static_cast<::jxx::lang::jlong>(result);
}
void RandomAccessFile::setLength(::jxx::lang::jlong newLength) {
    std::lock_guard<std::recursive_mutex> lock(mutex_); ensureWritable_();
    if (newLength < 0) throw ::jxx::lang::IllegalArgumentException();
    const auto current = getFilePointer();
    if (std::fflush(handle_) != 0) throw IOException();
#if defined(_WIN32)
    if (_chsize_s(_fileno(handle_), static_cast<__int64>(newLength)) != 0) throw IOException();
#else
    if (::ftruncate(fileno(handle_), static_cast<off_t>(newLength)) != 0) throw IOException();
#endif
    seek(std::min(current, newLength));
    syncIfRequired_();
}
void RandomAccessFile::close() {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    if (channel_ != nullptr) { channel_->close(); channel_.reset(); }
    if (handle_ != nullptr) {
        std::fflush(handle_);
        std::fclose(handle_);
        handle_ = nullptr;
        descriptor_.reset();
    }
}

} // namespace jxx::io
