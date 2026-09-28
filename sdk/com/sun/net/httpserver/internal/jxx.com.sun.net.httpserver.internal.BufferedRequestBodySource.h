#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.RequestBodySource.h"

namespace jxx::com::sun::net::httpserver::internal {

class BufferedRequestBodySource final
    : public ::jxx::lang::ClassBase<BufferedRequestBodySource, RequestBodySource> {
public:
    using JxxSuper = RequestBodySource;
    using Super = ::jxx::lang::ClassBase<BufferedRequestBodySource, JxxSuper>;

    BufferedRequestBodySource(
        const ::jxx::lang::ByteArray& body,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length);

    ::jxx::lang::jint read(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;
    ::jxx::lang::jlong skip(::jxx::lang::jlong count) override;
    ::jxx::lang::jint available() override;
    void close() override;
    ::jxx::lang::jbool isFullyConsumedInternal() const noexcept override;
    ::jxx::lang::jbool wasClosedInternal() const noexcept override;

private:
    ::jxx::lang::ByteArray body_;
    ::jxx::lang::jint position_ = 0;
    ::jxx::lang::jint limit_ = 0;
    ::jxx::lang::jbool closed_ = false;
};

} // namespace jxx::com::sun::net::httpserver::internal
