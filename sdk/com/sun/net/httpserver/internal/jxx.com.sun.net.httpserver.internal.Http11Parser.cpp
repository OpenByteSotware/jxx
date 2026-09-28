#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.Http11Parser.h"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <limits>

namespace jxx::com::sun::net::httpserver::internal {
namespace {

std::string lower(std::string value)
{
    std::transform(
        value.begin(), value.end(), value.begin(),
        [](unsigned char character) {
            return static_cast<char>(std::tolower(character));
        });
    return value;
}

std::string trim(std::string value)
{
    while (!value.empty() &&
           (value.front() == ' ' || value.front() == '\t')) {
        value.erase(value.begin());
    }
    while (!value.empty() &&
           (value.back() == ' ' || value.back() == '\t')) {
        value.pop_back();
    }
    return value;
}

bool parseHeaderLine(
    const std::string& source,
    std::size_t begin,
    std::size_t end,
    std::size_t maxNameBytes,
    std::size_t maxValueBytes,
    std::pair<std::string, std::string>& output)
{
    const auto colon = source.find(':', begin);
    if (colon == std::string::npos || colon >= end || colon == begin) {
        return false;
    }
    if (colon - begin > maxNameBytes || end - colon - 1 > maxValueBytes) return false;
    output.first = source.substr(begin, colon - begin);
    output.second = trim(source.substr(colon + 1, end - colon - 1));
    return output.second.size() <= maxValueBytes;
}

bool isTokenCharacter(unsigned char character)
{
    if (std::isalnum(character) != 0) return true;
    switch (character) {
    case '!': case '#': case '$': case '%': case '&': case '\'':
    case '*': case '+': case '-': case '.': case '^': case '_':
    case '`': case '|': case '~': return true;
    default: return false;
    }
}

bool parseTransferEncoding(
    const std::vector<std::pair<std::string, std::string>>& headers,
    bool& chunked,
    std::string& error)
{
    chunked = false;
    std::vector<std::string> codings;
    for (const auto& header : headers) {
        if (lower(header.first) != "transfer-encoding") continue;
        const std::string value = lower(header.second);
        std::size_t position = 0;
        for (;;) {
            const auto comma = value.find(',', position);
            const auto token = trim(value.substr(position, comma == std::string::npos ? std::string::npos : comma - position));
            if (token.empty()) { error = "empty transfer coding"; return false; }
            if (!std::all_of(token.begin(), token.end(), [](unsigned char c) { return isTokenCharacter(c); })) {
                error = "invalid transfer coding"; return false;
            }
            codings.push_back(token);
            if (comma == std::string::npos) break;
            position = comma + 1;
        }
    }
    if (codings.empty()) return true;
    std::size_t chunkedCount = 0;
    for (std::size_t index = 0; index < codings.size(); ++index) {
        if (codings[index] == "chunked") {
            ++chunkedCount;
            if (index + 1U != codings.size()) { error = "chunked must be final"; return false; }
        }
        else { error = "unsupported transfer coding"; return false; }
    }
    if (chunkedCount != 1U) { error = "invalid chunked transfer coding"; return false; }
    chunked = true;
    return true;
}

Http11Parser::Result decodeChunked(
    const std::string& source,
    std::size_t bodyStart,
    ParsedRequest& output,
    std::size_t& consumed,
    std::string& error,
    const HttpServerLimits& limits)
{
    std::size_t position = bodyStart;
    output.body.clear();
    output.trailers.clear();

    for (;;) {
        const auto lineEnd = source.find("\r\n", position);
        if (lineEnd == std::string::npos) return Http11Parser::Result::NeedMore;

        std::string sizeText = source.substr(position, lineEnd - position);
        const auto extension = sizeText.find(';');
        if (extension != std::string::npos) sizeText.erase(extension);
        sizeText = trim(sizeText);
        if (sizeText.empty()) {
            error = "invalid chunk size";
            return Http11Parser::Result::Error;
        }

        errno = 0;
        char* end = nullptr;
        const auto chunkSize = std::strtoull(sizeText.c_str(), &end, 16);
        if (errno == ERANGE || end == nullptr || *end != '\0' ||
            chunkSize > std::numeric_limits<std::size_t>::max()) {
            error = "invalid chunk size";
            return Http11Parser::Result::Error;
        }

        position = lineEnd + 2;
        if (chunkSize == 0) {
            for (;;) {
                const auto trailerEnd = source.find("\r\n", position);
                if (trailerEnd == std::string::npos)
                    return Http11Parser::Result::NeedMore;
                if (trailerEnd == position) {
                    consumed = trailerEnd + 2;
                    return Http11Parser::Result::Complete;
                }
                std::pair<std::string, std::string> trailer;
                if (!parseHeaderLine(source, position, trailerEnd, limits.maxHeaderNameBytes, limits.maxHeaderValueBytes, trailer)) {
                    error = "invalid trailer";
                    return Http11Parser::Result::Error;
                }
                const auto name = lower(trailer.first);
                if (name == "content-length" ||
                    name == "transfer-encoding" ||
                    name == "host") {
                    error = "forbidden trailer";
                    return Http11Parser::Result::Error;
                }
                output.trailers.push_back(std::move(trailer));
                position = trailerEnd + 2;
            }
        }

        if (chunkSize > source.size() - position)
            return Http11Parser::Result::NeedMore;
        const auto dataEnd = position + static_cast<std::size_t>(chunkSize);
        if (source.size() < dataEnd + 2)
            return Http11Parser::Result::NeedMore;
        if (source.compare(dataEnd, 2, "\r\n") != 0) {
            error = "invalid chunk terminator";
            return Http11Parser::Result::Error;
        }

        output.body.insert(
            output.body.end(),
            reinterpret_cast<const unsigned char*>(source.data() + position),
            reinterpret_cast<const unsigned char*>(source.data() + dataEnd));
        position = dataEnd + 2;
    }
}

} // namespace

bool Http11Parser::parseContentLength(
    const std::vector<std::pair<std::string, std::string>>& headers,
    std::size_t& value,
    bool& present)
{
    value = 0;
    present = false;
    for (const auto& header : headers) {
        if (lower(header.first) != "content-length") continue;
        errno = 0;
        char* end = nullptr;
        const auto parsed = std::strtoull(header.second.c_str(), &end, 10);
        if (errno == ERANGE || end == nullptr || *end != '\0' ||
            parsed > std::numeric_limits<std::size_t>::max()) {
            return false;
        }
        if (present && value != static_cast<std::size_t>(parsed)) return false;
        value = static_cast<std::size_t>(parsed);
        present = true;
    }
    return true;
}

Http11Parser::Result Http11Parser::parse(
    const unsigned char* data,
    std::size_t size,
    ParsedRequest& output,
    std::size_t& consumed,
    std::string& error) const
{
    consumed = 0;
    output = ParsedRequest{};
    const std::string source(reinterpret_cast<const char*>(data), size);
    const auto headersEnd = source.find("\r\n\r\n");
    if (headersEnd == std::string::npos) return Result::NeedMore;

    const auto requestLineEnd = source.find("\r\n");
    if (requestLineEnd == std::string::npos) return Result::NeedMore;
    const auto firstSpace = source.find(' ');
    const auto secondSpace = source.find(' ', firstSpace + 1);
    if (firstSpace == std::string::npos || secondSpace == std::string::npos ||
        secondSpace >= requestLineEnd) {
        error = "bad request line";
        return Result::Error;
    }

    output.method = source.substr(0, firstSpace);
    output.target = source.substr(firstSpace + 1, secondSpace - firstSpace - 1);
    output.version = source.substr(secondSpace + 1, requestLineEnd - secondSpace - 1);
    if (output.version != "HTTP/1.0" && output.version != "HTTP/1.1") {
        error = "unsupported HTTP version";
        return Result::Error;
    }

    std::size_t position = requestLineEnd + 2;
    while (position < headersEnd) {
        const auto lineEnd = source.find("\r\n", position);
        std::pair<std::string, std::string> header;
        if (lineEnd == std::string::npos || lineEnd > headersEnd ||
            !parseHeaderLine(source, position, lineEnd, limits_.maxHeaderNameBytes, limits_.maxHeaderValueBytes, header)) {
            error = "bad header";
            return Result::Error;
        }
        output.headers.push_back(std::move(header));
        position = lineEnd + 2;
    }

    std::size_t contentLength = 0;
    bool contentLengthPresent = false;
    if (!parseContentLength(output.headers, contentLength, contentLengthPresent)) {
        error = "invalid content-length";
        return Result::Error;
    }

    bool chunked = false;
    if (!parseTransferEncoding(output.headers, chunked, error)) return Result::Error;
    if (chunked && output.version == "HTTP/1.0") {
        error = "chunked transfer coding requires HTTP/1.1";
        return Result::Error;
    }
    if (chunked && contentLengthPresent) {
        error = "conflicting transfer framing";
        return Result::Error;
    }

    const auto bodyStart = headersEnd + 4;
    if (chunked) {
        const auto result = decodeChunked(
            source, bodyStart, output, consumed, error, limits_);
        if (result != Result::Complete) return result;
    }
    else {
        if (source.size() - bodyStart < contentLength) return Result::NeedMore;
        output.body.assign(
            data + bodyStart,
            data + bodyStart + contentLength);
        consumed = bodyStart + contentLength;
    }

    output.keepAlive = output.version == "HTTP/1.1";
    bool connectionClose = false;
    bool connectionKeepAlive = false;
    for (const auto& header : output.headers) {
        if (lower(header.first) != "connection") continue;
        const auto value = lower(header.second);
        std::size_t position = 0;
        for (;;) {
            const auto comma = value.find(',', position);
            const auto token = trim(value.substr(position, comma == std::string::npos ? std::string::npos : comma - position));
            if (token == "close") connectionClose = true;
            else if (token == "keep-alive") connectionKeepAlive = true;
            if (comma == std::string::npos) break;
            position = comma + 1;
        }
    }
    if (connectionClose) output.keepAlive = false;
    else if (output.version == "HTTP/1.0" && connectionKeepAlive) output.keepAlive = true;
    return Result::Complete;
}

} // namespace jxx::com::sun::net::httpserver::internal
