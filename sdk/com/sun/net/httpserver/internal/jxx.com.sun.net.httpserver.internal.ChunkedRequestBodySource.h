#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.HttpServerLimits.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.RequestBodySource.h"

#include <string>

namespace jxx::io { class InputStream; }

namespace jxx::com::sun::net::httpserver::internal {

class ChunkedRequestBodySource final
    : public ::jxx::lang::ClassBase<ChunkedRequestBodySource, RequestBodySource> {
public:
    using JxxSuper = RequestBodySource;
    using Super = ::jxx::lang::ClassBase<ChunkedRequestBodySource, JxxSuper>;

    ChunkedRequestBodySource(
        const ::jxx::Ptr<::jxx::io::InputStream>& input,
        const ::jxx::lang::ByteArray& prefix,
        ::jxx::lang::jint prefixOffset,
        ::jxx::lang::jint prefixLength,
        const HttpServerLimits& limits = HttpServerLimits());

    ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset, ::jxx::lang::jint length) override;
    ::jxx::lang::jlong skip(::jxx::lang::jlong count) override;
    ::jxx::lang::jint available() override;
    void close() override;
    ::jxx::lang::jbool isFullyConsumedInternal() const noexcept override;
    ::jxx::lang::jbool wasClosedInternal() const noexcept override;

private:
    ::jxx::lang::jint readRawByte_();
    std::string readLine_(std::size_t limit, const char* failure);
    void beginChunk_();
    void consumeChunkTerminator_();
    void consumeTrailers_();

    ::jxx::Ptr<::jxx::io::InputStream> input_;
    ::jxx::lang::ByteArray prefix_;
    ::jxx::lang::jint prefixPosition_ = 0;
    ::jxx::lang::jint prefixLimit_ = 0;
    HttpServerLimits limits_;
    std::size_t chunkRemaining_ = 0;
    std::size_t decodedBytes_ = 0;
    ::jxx::lang::jbool needChunkTerminator_ = false;
    ::jxx::lang::jbool finished_ = false;
    ::jxx::lang::jbool closed_ = false;
};

} // namespace jxx::com::sun::net::httpserver::internal
