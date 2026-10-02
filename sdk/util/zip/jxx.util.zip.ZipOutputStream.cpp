#include "util/zip/jxx.util.zip.ZipOutputStream.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <limits>

#include <zlib.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/zip/jxx.util.zip.ZipEntry.h"
#include "util/zip/jxx.util.zip.ZipException.h"

namespace {

std::uint32_t crc32Of(const std::vector<std::uint8_t>& data) {
    uLong crc = ::crc32(0L, Z_NULL, 0);
    if (!data.empty()) {
        crc = ::crc32(crc,
                      reinterpret_cast<const Bytef*>(data.data()),
                      static_cast<uInt>(data.size()));
    }
    return static_cast<std::uint32_t>(crc);
}

std::vector<std::uint8_t> deflateRaw(const std::vector<std::uint8_t>& input, int level) {
    z_stream stream{};
    if (::deflateInit2(&stream, level, Z_DEFLATED, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        throw ::jxx::util::zip::ZipException("unable to initialize DEFLATE compressor");
    }

    std::vector<std::uint8_t> output;
    output.resize(std::max<std::size_t>(128, input.size() / 2 + 64));
    stream.next_in = input.empty() ? Z_NULL : const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input.data()));
    stream.avail_in = static_cast<uInt>(input.size());

    int result = Z_OK;
    do {
        if (stream.total_out == output.size()) {
            output.resize(output.size() * 2);
        }
        stream.next_out = reinterpret_cast<Bytef*>(output.data() + stream.total_out);
        stream.avail_out = static_cast<uInt>(output.size() - stream.total_out);
        result = ::deflate(&stream, Z_FINISH);
    } while (result == Z_OK);

    const auto produced = static_cast<std::size_t>(stream.total_out);
    ::deflateEnd(&stream);
    if (result != Z_STREAM_END) {
        throw ::jxx::util::zip::ZipException("DEFLATE compression failed");
    }
    output.resize(produced);
    return output;
}

void requireZip32(std::size_t value, const char* message) {
    if (value > std::numeric_limits<std::uint32_t>::max()) {
        throw ::jxx::util::zip::ZipException(message);
    }
}

std::pair<std::uint16_t, std::uint16_t> dosDateTime(::jxx::lang::jlong millis) {
    const std::time_t seconds = static_cast<std::time_t>(millis / 1000);
    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &seconds);
#else
    localtime_r(&seconds, &local);
#endif
    int year = local.tm_year + 1900;
    if (year < 1980) {
        year = 1980;
        local.tm_mon = 0;
        local.tm_mday = 1;
        local.tm_hour = 0;
        local.tm_min = 0;
        local.tm_sec = 0;
    } else if (year > 2107) {
        year = 2107;
        local.tm_mon = 11;
        local.tm_mday = 31;
        local.tm_hour = 23;
        local.tm_min = 59;
        local.tm_sec = 58;
    }
    const auto time = static_cast<std::uint16_t>(
        (local.tm_hour << 11) | (local.tm_min << 5) | (local.tm_sec / 2));
    const auto date = static_cast<std::uint16_t>(
        ((year - 1980) << 9) | ((local.tm_mon + 1) << 5) | local.tm_mday);
    return {time, date};
}

} // namespace

namespace jxx::util::zip {

ZipOutputStream::ZipOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output)
    : Super(output) {
    if (output == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

ZipOutputStream::~ZipOutputStream() {
    try {
        close();
    } catch (...) {
    }
}

void ZipOutputStream::ensureOpen_() const {
    if (closed_) {
        throw ZipException("stream closed");
    }
}

void ZipOutputStream::ensureWritable_() const {
    ensureOpen_();
    if (finished_) {
        throw ZipException("stream has been finished");
    }
}

void ZipOutputStream::putNextEntry(const ::jxx::Ptr<ZipEntry>& entry) {
    ensureWritable_();
    if (entry == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (current_ != nullptr) {
        closeEntry();
    }
    for (const auto& record : records_) {
        if (record.entry->getName()->equals(entry->getName())) {
            throw ZipException("duplicate entry");
        }
    }
    current_ = ::jxx::NEW<ZipEntry>(entry);
    if (current_->time_ < 0) {
        current_->time_ = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }
    if (current_->method_ == -1) {
        current_->method_ = method_;
    }
    if (current_->method_ != STORED && current_->method_ != DEFLATED) {
        throw ZipException("unsupported compression method");
    }
    currentData_.clear();
}

void ZipOutputStream::closeEntry() {
    ensureWritable_();
    if (current_ == nullptr) {
        throw ZipException("no current ZIP entry");
    }

    requireZip32(currentData_.size(), "ZIP64 entry size is not supported");
    Record record;
    record.entry = current_;
    record.crc = crc32Of(currentData_);
    const auto declaredSize = record.entry->size_;
    const auto declaredCrc = record.entry->crc_;

    if (record.entry->method_ == STORED) {
        if (declaredSize < 0 || declaredCrc < 0) {
            throw ZipException("STORED entry missing size, compressed size, or CRC");
        }
        if (declaredSize != static_cast<::jxx::lang::jlong>(currentData_.size())) {
            throw ZipException("invalid STORED entry size");
        }
        if (declaredCrc != record.crc) {
            throw ZipException("invalid STORED entry CRC");
        }
        record.data = currentData_;
    } else {
        record.data = deflateRaw(currentData_, level_);
    }

    requireZip32(record.data.size(), "ZIP64 compressed size is not supported");
    record.entry->size_ = static_cast<::jxx::lang::jlong>(currentData_.size());
    record.entry->compressedSize_ = static_cast<::jxx::lang::jlong>(record.data.size());
    record.entry->crc_ = record.crc;
    records_.push_back(std::move(record));
    current_.reset();
    currentData_.clear();
}

void ZipOutputStream::write(::jxx::lang::jint value) {
    ensureWritable_();
    if (current_ == nullptr) {
        throw ZipException("no current ZIP entry");
    }
    currentData_.push_back(static_cast<std::uint8_t>(value));
}

void ZipOutputStream::write(const ::jxx::lang::ByteArray& buffer) {
    if (buffer == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    write(buffer, 0, buffer->length);
}

void ZipOutputStream::write(const ::jxx::lang::ByteArray& buffer,
                            ::jxx::lang::jint offset,
                            ::jxx::lang::jint length) {
    ensureWritable_();
    if (buffer == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (offset < 0 || length < 0 || offset > buffer->length - length) {
        throw ::jxx::lang::IndexOutOfBoundsException();
    }
    if (current_ == nullptr) {
        throw ZipException("no current ZIP entry");
    }
    for (::jxx::lang::jint index = 0; index < length; ++index) {
        currentData_.push_back(static_cast<std::uint8_t>((*buffer)[offset + index]));
    }
}

void ZipOutputStream::emit_(std::uint8_t value) {
    out_->write(static_cast<::jxx::lang::jint>(value));
    ++position_;
}

void ZipOutputStream::emit16_(std::uint16_t value) {
    emit_(static_cast<std::uint8_t>(value));
    emit_(static_cast<std::uint8_t>(value >> 8));
}

void ZipOutputStream::emit32_(std::uint32_t value) {
    emit16_(static_cast<std::uint16_t>(value));
    emit16_(static_cast<std::uint16_t>(value >> 16));
}

void ZipOutputStream::emitBytes_(const std::string& value) {
    for (const unsigned char byte : value) {
        emit_(byte);
    }
}

void ZipOutputStream::emitBytes_(const std::vector<std::uint8_t>& value) {
    for (const auto byte : value) {
        emit_(byte);
    }
}

void ZipOutputStream::finish() {
    ensureOpen_();
    if (finished_) {
        return;
    }
    if (current_ != nullptr) {
        closeEntry();
    }
    if (records_.size() > 0xffffU) {
        throw ZipException("ZIP64 entry count is not supported");
    }

    for (auto& record : records_) {
        const auto name = record.entry->getName()->utf8();
        const auto extra = record.entry->extra_;
        const auto extraLength = extra == nullptr ? 0U : static_cast<std::uint16_t>(extra->length);
        record.offset = position_;
        const auto dos = dosDateTime(record.entry->time_);
        emit32_(0x04034b50U);
        emit16_(20);
        emit16_(0x0800U);
        emit16_(static_cast<std::uint16_t>(record.entry->method_));
        emit16_(dos.first);
        emit16_(dos.second);
        emit32_(record.crc);
        emit32_(static_cast<std::uint32_t>(record.data.size()));
        emit32_(static_cast<std::uint32_t>(record.entry->size_));
        emit16_(static_cast<std::uint16_t>(name.size()));
        emit16_(extraLength);
        emitBytes_(name);
        if (extra != nullptr) {
            for (::jxx::lang::jint index = 0; index < extra->length; ++index) {
                emit_(static_cast<std::uint8_t>((*extra)[index]));
            }
        }
        emitBytes_(record.data);
    }

    const auto centralOffset = position_;
    for (const auto& record : records_) {
        const auto name = record.entry->getName()->utf8();
        const auto comment = record.entry->comment_ == nullptr ? std::string() : record.entry->comment_->utf8();
        const auto extra = record.entry->extra_;
        const auto extraLength = extra == nullptr ? 0U : static_cast<std::uint16_t>(extra->length);
        const auto dos = dosDateTime(record.entry->time_);
        emit32_(0x02014b50U);
        emit16_(20);
        emit16_(20);
        emit16_(0x0800U);
        emit16_(static_cast<std::uint16_t>(record.entry->method_));
        emit16_(dos.first);
        emit16_(dos.second);
        emit32_(record.crc);
        emit32_(static_cast<std::uint32_t>(record.data.size()));
        emit32_(static_cast<std::uint32_t>(record.entry->size_));
        emit16_(static_cast<std::uint16_t>(name.size()));
        emit16_(extraLength);
        emit16_(static_cast<std::uint16_t>(comment.size()));
        emit16_(0);
        emit16_(0);
        emit32_(record.entry->isDirectory() ? 0x10U : 0U);
        emit32_(record.offset);
        emitBytes_(name);
        if (extra != nullptr) {
            for (::jxx::lang::jint index = 0; index < extra->length; ++index) {
                emit_(static_cast<std::uint8_t>((*extra)[index]));
            }
        }
        emitBytes_(comment);
    }

    const auto centralSize = position_ - centralOffset;
    const auto archiveComment = comment_ == nullptr ? std::string() : comment_->utf8();
    emit32_(0x06054b50U);
    emit16_(0);
    emit16_(0);
    emit16_(static_cast<std::uint16_t>(records_.size()));
    emit16_(static_cast<std::uint16_t>(records_.size()));
    emit32_(centralSize);
    emit32_(centralOffset);
    emit16_(static_cast<std::uint16_t>(archiveComment.size()));
    emitBytes_(archiveComment);
    out_->flush();
    finished_ = true;
}

void ZipOutputStream::close() {
    if (closed_) {
        return;
    }
    finish();
    out_->close();
    closed_ = true;
}

void ZipOutputStream::setMethod(::jxx::lang::jint method) {
    ensureWritable_();
    if (method != STORED && method != DEFLATED) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    method_ = method;
}

void ZipOutputStream::setLevel(::jxx::lang::jint level) {
    ensureWritable_();
    if (level < -1 || level > 9) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    level_ = level;
}

void ZipOutputStream::setComment(const ::jxx::Ptr<::jxx::lang::String>& comment) {
    ensureWritable_();
    if (comment != nullptr && comment->utf8().size() > 0xffffU) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    comment_ = comment;
}

} // namespace jxx::util::zip
