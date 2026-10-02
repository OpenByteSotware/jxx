#include "util/zip/jxx.util.zip.ZipFile.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <limits>

#include <zlib.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.File.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.VectorEnumeration.h"
#include "util/zip/jxx.util.zip.ZipEntry.h"
#include "util/zip/jxx.util.zip.ZipException.h"

namespace {

::jxx::lang::jlong dosToMillis(std::uint16_t date, std::uint16_t time) {
    if (date == 0U) {
        return -1;
    }
    std::tm local{};
    local.tm_year = static_cast<int>(((date >> 9U) & 0x7fU) + 1980U) - 1900;
    local.tm_mon = static_cast<int>((date >> 5U) & 0x0fU) - 1;
    local.tm_mday = static_cast<int>(date & 0x1fU);
    local.tm_hour = static_cast<int>((time >> 11U) & 0x1fU);
    local.tm_min = static_cast<int>((time >> 5U) & 0x3fU);
    local.tm_sec = static_cast<int>((time & 0x1fU) * 2U);
    local.tm_isdst = -1;
    const auto seconds = std::mktime(&local);
    return seconds == static_cast<std::time_t>(-1)
        ? -1
        : static_cast<::jxx::lang::jlong>(seconds) * 1000;
}

std::uint32_t crc32Of(const std::vector<std::uint8_t>& data) {
    uLong crc = ::crc32(0L, Z_NULL, 0);
    if (!data.empty()) {
        crc = ::crc32(crc,
                      reinterpret_cast<const Bytef*>(data.data()),
                      static_cast<uInt>(data.size()));
    }
    return static_cast<std::uint32_t>(crc);
}

std::vector<std::uint8_t> inflateRaw(const std::uint8_t* compressed,
                                     std::size_t compressedSize,
                                     std::size_t expectedSize) {
    z_stream stream{};
    if (::inflateInit2(&stream, -MAX_WBITS) != Z_OK) {
        throw ::jxx::util::zip::ZipException("unable to initialize DEFLATE decompressor");
    }

    std::vector<std::uint8_t> output(std::max<std::size_t>(expectedSize, 1U));
    stream.next_in = compressedSize == 0 ? Z_NULL : const_cast<Bytef*>(reinterpret_cast<const Bytef*>(compressed));
    stream.avail_in = static_cast<uInt>(compressedSize);

    int result = Z_OK;
    do {
        if (stream.total_out == output.size()) {
            if (output.size() > std::numeric_limits<std::size_t>::max() / 2U) {
                ::inflateEnd(&stream);
                throw ::jxx::util::zip::ZipException("inflated entry is too large");
            }
            output.resize(output.size() * 2U);
        }
        stream.next_out = reinterpret_cast<Bytef*>(output.data() + stream.total_out);
        stream.avail_out = static_cast<uInt>(output.size() - stream.total_out);
        result = ::inflate(&stream, Z_NO_FLUSH);
    } while (result == Z_OK);

    const auto produced = static_cast<std::size_t>(stream.total_out);
    ::inflateEnd(&stream);
    if (result != Z_STREAM_END) {
        throw ::jxx::util::zip::ZipException("invalid or truncated DEFLATE entry");
    }
    if (produced != expectedSize) {
        throw ::jxx::util::zip::ZipException("invalid uncompressed entry size");
    }
    output.resize(produced);
    return output;
}

} // namespace

namespace jxx::util::zip {

ZipFile::ZipFile(const ::jxx::Ptr<::jxx::lang::String>& name) {
    open_(name);
}

ZipFile::ZipFile(const ::jxx::Ptr<::jxx::io::File>& file) {
    if (file == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    open_(file->getPath());
}

ZipFile::~ZipFile() {
    try {
        close();
    } catch (...) {
    }
}

std::uint16_t ZipFile::u16_(std::size_t offset) const {
    if (offset + 2U > bytes_.size()) {
        throw ZipException("truncated ZIP file");
    }
    return static_cast<std::uint16_t>(bytes_[offset]) |
           static_cast<std::uint16_t>(bytes_[offset + 1U] << 8U);
}

std::uint32_t ZipFile::u32_(std::size_t offset) const {
    if (offset + 4U > bytes_.size()) {
        throw ZipException("truncated ZIP file");
    }
    return static_cast<std::uint32_t>(u16_(offset)) |
           (static_cast<std::uint32_t>(u16_(offset + 2U)) << 16U);
}

void ZipFile::open_(const ::jxx::Ptr<::jxx::lang::String>& name) {
    if (name == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    name_ = name;
    std::ifstream input(std::filesystem::u8path(name->utf8()), std::ios::binary);
    if (!input) {
        throw ZipException("unable to open ZIP file");
    }
    bytes_.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    if (bytes_.size() < 22U) {
        throw ZipException("invalid ZIP file");
    }

    const auto searchStart = bytes_.size() > 0xffffU + 22U ? bytes_.size() - (0xffffU + 22U) : 0U;
    std::size_t endOffset = bytes_.size();
    for (std::size_t offset = bytes_.size() - 22U;; --offset) {
        if (u32_(offset) == 0x06054b50U) {
            endOffset = offset;
            break;
        }
        if (offset == searchStart) {
            break;
        }
    }
    if (endOffset == bytes_.size()) {
        throw ZipException("missing end of central directory");
    }

    const auto diskNumber = u16_(endOffset + 4U);
    const auto centralDisk = u16_(endOffset + 6U);
    const auto entriesOnDisk = u16_(endOffset + 8U);
    const auto entryCount = u16_(endOffset + 10U);
    const auto centralSize = u32_(endOffset + 12U);
    const auto centralOffset = u32_(endOffset + 16U);
    const auto commentLength = u16_(endOffset + 20U);
    if (diskNumber != 0 || centralDisk != 0 || entriesOnDisk != entryCount) {
        throw ZipException("multi-disk ZIP files are not supported");
    }
    if (entryCount == 0xffffU || centralSize == 0xffffffffU || centralOffset == 0xffffffffU) {
        throw ZipException("ZIP64 is not supported");
    }
    if (endOffset + 22U + commentLength > bytes_.size()) {
        throw ZipException("truncated ZIP comment");
    }
    if (commentLength != 0U) {
        comment_ = ::jxx::NEW<::jxx::lang::String>(
            std::string(reinterpret_cast<const char*>(bytes_.data() + endOffset + 22U), commentLength));
    }
    if (static_cast<std::uint64_t>(centralOffset) + centralSize > endOffset) {
        throw ZipException("invalid central directory bounds");
    }

    std::size_t position = centralOffset;
    for (std::uint16_t index = 0; index < entryCount; ++index) {
        if (u32_(position) != 0x02014b50U || position + 46U > bytes_.size()) {
            throw ZipException("invalid central directory entry");
        }
        const auto flags = u16_(position + 8U);
        const auto method = u16_(position + 10U);
        const auto modifiedTime = u16_(position + 12U);
        const auto modifiedDate = u16_(position + 14U);
        const auto crc = u32_(position + 16U);
        const auto compressedSize = u32_(position + 20U);
        const auto size = u32_(position + 24U);
        const auto nameLength = u16_(position + 28U);
        const auto extraLength = u16_(position + 30U);
        const auto entryCommentLength = u16_(position + 32U);
        const auto localOffset = u32_(position + 42U);
        const auto recordEnd = position + 46U + nameLength + extraLength + entryCommentLength;
        if (recordEnd > bytes_.size()) {
            throw ZipException("truncated central directory entry");
        }
        if ((flags & 0x0001U) != 0U) {
            throw ZipException("encrypted ZIP entries are not supported");
        }
        if (method != ZipEntry::STORED && method != ZipEntry::DEFLATED) {
            throw ZipException("unsupported ZIP compression method");
        }

        const std::string entryName(
            reinterpret_cast<const char*>(bytes_.data() + position + 46U), nameLength);
        auto entry = ::jxx::NEW<ZipEntry>(::jxx::NEW<::jxx::lang::String>(entryName));
        entry->method_ = method;
        entry->time_ = dosToMillis(modifiedDate, modifiedTime);
        entry->crc_ = crc;
        entry->compressedSize_ = compressedSize;
        entry->size_ = size;
        entry->localOffset_ = localOffset;
        if (extraLength != 0U) {
            entry->extra_ = ::jxx::NEW<::jxx::lang::ByteArrayType>(extraLength);
            for (::jxx::lang::jint i = 0; i < extraLength; ++i) {
                (*entry->extra_)[i] = static_cast<::jxx::lang::jbyte>(bytes_[position + 46U + nameLength + i]);
            }
        }
        if (entryCommentLength != 0U) {
            entry->comment_ = ::jxx::NEW<::jxx::lang::String>(
                std::string(reinterpret_cast<const char*>(bytes_.data() + position + 46U + nameLength + extraLength),
                            entryCommentLength));
        }

        if (localOffset + 30U > bytes_.size() || u32_(localOffset) != 0x04034b50U) {
            throw ZipException("invalid local ZIP header");
        }
        const auto localFlags = u16_(localOffset + 6U);
        const auto localMethod = u16_(localOffset + 8U);
        const auto localNameLength = u16_(localOffset + 26U);
        const auto localExtraLength = u16_(localOffset + 28U);
        if ((localFlags & 0x0001U) != 0U || localMethod != method) {
            throw ZipException("inconsistent local ZIP header");
        }
        if (localOffset + 30U + localNameLength + localExtraLength > bytes_.size()) {
            throw ZipException("truncated local ZIP header");
        }
        const std::string localName(
            reinterpret_cast<const char*>(bytes_.data() + localOffset + 30U), localNameLength);
        if (localName != entryName) {
            throw ZipException("inconsistent local ZIP entry name");
        }
        entry->dataOffset_ = static_cast<::jxx::lang::jlong>(localOffset + 30U + localNameLength + localExtraLength);
        if (static_cast<std::uint64_t>(entry->dataOffset_) + compressedSize > bytes_.size()) {
            throw ZipException("truncated ZIP entry data");
        }
        entries_.push_back(entry);
        position = recordEnd;
    }
}

void ZipFile::ensureOpen_() const {
    if (closed_) {
        throw ZipException("ZIP file closed");
    }
}

::jxx::Ptr<ZipEntry> ZipFile::getEntry(const ::jxx::Ptr<::jxx::lang::String>& name) const {
    ensureOpen_();
    if (name == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    for (const auto& entry : entries_) {
        if (entry->getName()->equals(name)) {
            return ::jxx::NEW<ZipEntry>(entry);
        }
    }
    return nullptr;
}

::jxx::Ptr<::jxx::util::Enumeration<ZipEntry>> ZipFile::entries() {
    ensureOpen_();
    std::vector<::jxx::Ptr<ZipEntry>> copy;
    copy.reserve(entries_.size());
    for (const auto& entry : entries_) {
        copy.push_back(::jxx::NEW<ZipEntry>(entry));
    }
    return ::jxx::NEW<::jxx::util::VectorEnumeration<ZipEntry>>(std::move(copy));
}

::jxx::Ptr<::jxx::io::InputStream> ZipFile::getInputStream(const ::jxx::Ptr<ZipEntry>& entry) {
    ensureOpen_();
    if (entry == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    const auto original = getEntry(entry->getName());
    if (original == nullptr) {
        return nullptr;
    }

    const auto start = static_cast<std::size_t>(original->dataOffset_);
    const auto compressedSize = static_cast<std::size_t>(original->compressedSize_);
    const auto uncompressedSize = static_cast<std::size_t>(original->size_);
    if (start + compressedSize > bytes_.size()) {
        throw ZipException("truncated ZIP entry data");
    }

    std::vector<std::uint8_t> data;
    if (original->method_ == ZipEntry::STORED) {
        if (compressedSize != uncompressedSize) {
            throw ZipException("invalid STORED entry size");
        }
        data.assign(bytes_.begin() + start, bytes_.begin() + start + compressedSize);
    } else if (original->method_ == ZipEntry::DEFLATED) {
        data = inflateRaw(bytes_.data() + start, compressedSize, uncompressedSize);
    } else {
        throw ZipException("unsupported ZIP compression method");
    }

    if (crc32Of(data) != static_cast<std::uint32_t>(original->crc_)) {
        throw ZipException("invalid ZIP entry CRC");
    }
    auto array = ::jxx::NEW<::jxx::lang::ByteArrayType>(static_cast<::jxx::lang::jint>(data.size()));
    for (std::size_t index = 0; index < data.size(); ++index) {
        (*array)[static_cast<::jxx::lang::jint>(index)] = static_cast<::jxx::lang::jbyte>(data[index]);
    }
    return ::jxx::CAST<::jxx::io::InputStream>(::jxx::NEW<::jxx::io::ByteArrayInputStream>(array));
}

::jxx::Ptr<::jxx::lang::String> ZipFile::getName() const {
    return name_;
}

::jxx::lang::jint ZipFile::size() const noexcept {
    return static_cast<::jxx::lang::jint>(entries_.size());
}

::jxx::Ptr<::jxx::lang::String> ZipFile::getComment() const {
    return comment_;
}

void ZipFile::close() {
    if (closed_) {
        return;
    }
    closed_ = true;
    bytes_.clear();
    entries_.clear();
}

} // namespace jxx::util::zip
