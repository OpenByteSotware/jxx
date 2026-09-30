#pragma once

#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <limits>

namespace jxx::com::sun::net::httpserver::internal {

struct HttpServerLimits final {
    std::size_t maxRequestLineBytes = read_("JXX_HTTP_MAX_REQUEST_LINE_BYTES", 8192U);
    std::size_t maxHeaderBytes = read_("JXX_HTTP_MAX_HEADER_BYTES", 65536U);
    std::size_t maxHeaderCount = read_("JXX_HTTP_MAX_HEADER_COUNT", 100U);
    std::size_t maxHeaderNameBytes = read_("JXX_HTTP_MAX_HEADER_NAME_BYTES", 256U);
    std::size_t maxHeaderValueBytes = read_("JXX_HTTP_MAX_HEADER_VALUE_BYTES", 8192U);
    std::size_t maxChunkLineBytes = read_("JXX_HTTP_MAX_CHUNK_LINE_BYTES", 1024U);
    std::size_t maxTrailerBytes = read_("JXX_HTTP_MAX_TRAILER_BYTES", 16384U);
    std::size_t maxTrailerCount = read_("JXX_HTTP_MAX_TRAILER_COUNT", 32U);
    std::size_t maxDecodedBodyBytes = read_("JXX_HTTP_MAX_DECODED_BODY_BYTES", 16U * 1024U * 1024U);

private:
    static std::size_t read_(const char* name, std::size_t fallback) noexcept
    {
        const char* text = std::getenv(name);
        if (text == nullptr || *text == 0) return fallback;
        errno = 0;
        char* end = nullptr;
        const unsigned long long value = std::strtoull(text, &end, 10);
        if (errno != 0 || end == text || *end != 0 || value == 0ULL ||
            value > static_cast<unsigned long long>(std::numeric_limits<std::size_t>::max())) {
            return fallback;
        }
        return static_cast<std::size_t>(value);
    }
};

} // namespace jxx::com::sun::net::httpserver::internal
