#include <algorithm>
#include <cctype>
#include <limits>

#include "io/jxx.io.IOException.h"
#include "io/jxx.io.InputStream.h"
#include "lang/jxx.lang.Exceptions.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ChunkedRequestBodySource.h"


namespace jxx::com::sun::net::httpserver::internal {
namespace {

bool isTokenCharacter(unsigned char value)
{
    if (value <= 0x20U || value >= 0x7fU) return false;
    switch (value) {
    case '(': case ')': case '<': case '>': case '@': case ',': case ';':
    case ':': case '\\': case '"': case '/': case '[': case ']': case '?':
    case '=': case '{': case '}': return false;
    default: return true;
    }
}

std::string lowerAscii(std::string value)
{
    for (auto& character : value)
        if (character >= 'A' && character <= 'Z')
            character = static_cast<char>(character - 'A' + 'a');
    return value;
}

bool forbiddenTrailer(const std::string& name)
{
    const auto value = lowerAscii(name);
    return value == "content-length" || value == "transfer-encoding" ||
        value == "host" || value == "connection" || value == "trailer" ||
        value == "te" || value == "upgrade";
}

} // namespace

ChunkedRequestBodySource::ChunkedRequestBodySource(
    const ::jxx::Ptr<::jxx::io::InputStream>& input,
    const ::jxx::lang::ByteArray& prefix,
    ::jxx::lang::jint prefixOffset,
    ::jxx::lang::jint prefixLength,
    const HttpServerLimits& limits)
    : Super(), input_(input), prefix_(prefix), prefixPosition_(prefixOffset),
      prefixLimit_(prefixOffset + prefixLength), limits_(limits)
{
    if (input_ == nullptr || prefix_ == nullptr) throw ::jxx::lang::NullPointerException();
    if (prefixOffset < 0 || prefixLength < 0 || prefixOffset > prefix_->length - prefixLength)
        throw ::jxx::lang::IndexOutOfBoundsException();
}

::jxx::lang::jint ChunkedRequestBodySource::readRawByte_()
{
    if (prefixPosition_ < prefixLimit_)
        return static_cast<unsigned char>((*prefix_)[prefixPosition_++]);
    const auto value = input_->read();
    if (value < 0) throw ::jxx::io::IOException("unexpected end of chunked request body");
    return value;
}

std::string ChunkedRequestBodySource::readLine_(std::size_t limit, const char* failure)
{
    std::string line;
    for (;;) {
        const auto value = readRawByte_();
        if (value == '\r') {
            if (readRawByte_() != '\n') throw ::jxx::io::IOException(failure);
            return line;
        }
        if (value == '\n' || value == 0) throw ::jxx::io::IOException(failure);
        if (line.size() >= limit) throw ::jxx::io::IOException(failure);
        line.push_back(static_cast<char>(value));
    }
}

void ChunkedRequestBodySource::consumeChunkTerminator_()
{
    if (readRawByte_() != '\r' || readRawByte_() != '\n')
        throw ::jxx::io::IOException("invalid chunk terminator");
    needChunkTerminator_ = false;
}

void ChunkedRequestBodySource::consumeTrailers_()
{
    std::size_t count = 0;
    std::size_t bytes = 0;
    for (;;) {
        const auto line = readLine_(limits_.maxHeaderNameBytes + limits_.maxHeaderValueBytes + 1U,
            "invalid chunk trailer");
        if (line.empty()) return;
        bytes += line.size() + 2U;
        if (++count > limits_.maxTrailerCount || bytes > limits_.maxTrailerBytes)
            throw ::jxx::io::IOException("chunk trailer limit exceeded");
        const auto colon = line.find(':');
        if (colon == std::string::npos || colon == 0U || colon > limits_.maxHeaderNameBytes)
            throw ::jxx::io::IOException("invalid chunk trailer");
        const auto name = line.substr(0, colon);
        if (!std::all_of(name.begin(), name.end(), [](unsigned char value) { return isTokenCharacter(value); }))
            throw ::jxx::io::IOException("invalid chunk trailer");
        if (forbiddenTrailer(name)) throw ::jxx::io::IOException("forbidden chunk trailer");
        if (line.size() - colon - 1U > limits_.maxHeaderValueBytes)
            throw ::jxx::io::IOException("invalid chunk trailer");
        for (std::size_t index = colon + 1U; index < line.size(); ++index) {
            const auto value = static_cast<unsigned char>(line[index]);
            if ((value < 0x20U && value != '\t') || value == 0x7fU)
                throw ::jxx::io::IOException("invalid chunk trailer");
        }
    }
}

void ChunkedRequestBodySource::beginChunk_()
{
    if (needChunkTerminator_) consumeChunkTerminator_();
    const auto line = readLine_(limits_.maxChunkLineBytes, "invalid chunk-size line");
    const auto semicolon = line.find(';');
    const auto sizeText = line.substr(0, semicolon);
    if (sizeText.empty()) throw ::jxx::io::IOException("invalid chunk size");
    std::size_t size = 0;
    for (const auto character : sizeText) {
        unsigned int digit = 0;
        if (character >= '0' && character <= '9') digit = static_cast<unsigned int>(character - '0');
        else if (character >= 'a' && character <= 'f') digit = static_cast<unsigned int>(character - 'a' + 10);
        else if (character >= 'A' && character <= 'F') digit = static_cast<unsigned int>(character - 'A' + 10);
        else throw ::jxx::io::IOException("invalid chunk size");
        if (size > (std::numeric_limits<std::size_t>::max() - digit) / 16U)
            throw ::jxx::io::IOException("chunk size overflow");
        size = size * 16U + digit;
    }
    if (size > limits_.maxDecodedBodyBytes - decodedBytes_)
        throw ::jxx::io::IOException("request body too large");
    if (size == 0U) {
        consumeTrailers_();
        finished_ = true;
        return;
    }
    chunkRemaining_ = size;
}

::jxx::lang::jint ChunkedRequestBodySource::read(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length)
{
    if (buffer == nullptr) throw ::jxx::lang::NullPointerException();
    if (offset < 0 || length < 0 || offset > buffer->length - length)
        throw ::jxx::lang::IndexOutOfBoundsException();
    if (length == 0) return 0;
    if (closed_ || finished_) return -1;
    if (chunkRemaining_ == 0U) beginChunk_();
    if (finished_) return -1;

    const auto count = static_cast<::jxx::lang::jint>(
        std::min<std::size_t>(chunkRemaining_, static_cast<std::size_t>(length)));
    ::jxx::lang::jint copied = 0;
    while (copied < count && prefixPosition_ < prefixLimit_)
        (*buffer)[offset + copied++] = (*prefix_)[prefixPosition_++];
    if (copied < count) {
        const auto fromInput = input_->read(buffer, offset + copied, count - copied);
        if (fromInput < 0) throw ::jxx::io::IOException("unexpected end of chunk data");
        copied += fromInput;
    }
    chunkRemaining_ -= static_cast<std::size_t>(copied);
    decodedBytes_ += static_cast<std::size_t>(copied);
    if (chunkRemaining_ == 0U) needChunkTerminator_ = true;
    return copied;
}

::jxx::lang::jlong ChunkedRequestBodySource::skip(::jxx::lang::jlong count)
{
    if (count <= 0 || closed_ || finished_) return 0;
    auto scratch = ::jxx::NEW<::jxx::lang::ByteArrayType>(4096);
    ::jxx::lang::jlong skipped = 0;
    while (skipped < count) {
        const auto wanted = static_cast<::jxx::lang::jint>(
            std::min<::jxx::lang::jlong>(scratch->length, count - skipped));
        const auto actual = read(scratch, 0, wanted);
        if (actual < 0) break;
        skipped += actual;
    }
    return skipped;
}

::jxx::lang::jint ChunkedRequestBodySource::available()
{
    if (closed_ || finished_) return 0;
    const auto prefixAvailable = prefixLimit_ - prefixPosition_;
    if (chunkRemaining_ == 0U) return prefixAvailable;
    const auto immediatelyAvailable = prefixAvailable + std::max(0, input_->available());
    return static_cast<::jxx::lang::jint>(std::min<std::size_t>(chunkRemaining_,
        static_cast<std::size_t>(immediatelyAvailable)));
}

void ChunkedRequestBodySource::close()
{
    closed_ = true;
}

::jxx::lang::jbool ChunkedRequestBodySource::isFullyConsumedInternal() const noexcept
{
    return finished_;
}

::jxx::lang::jbool ChunkedRequestBodySource::wasClosedInternal() const noexcept
{
    return closed_;
}

} // namespace jxx::com::sun::net::httpserver::internal
