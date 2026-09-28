#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.RequestBodySource.h"

namespace jxx::io { class InputStream; }

namespace jxx::com::sun::net::httpserver::internal {

class FixedLengthRequestBodySource final
    : public ::jxx::lang::ClassBase<FixedLengthRequestBodySource, RequestBodySource> {
public:
    using JxxSuper = RequestBodySource;
    using Super = ::jxx::lang::ClassBase<FixedLengthRequestBodySource, JxxSuper>;

    FixedLengthRequestBodySource(
        const ::jxx::Ptr<::jxx::io::InputStream>& input,
        const ::jxx::lang::ByteArray& prefix,
        ::jxx::lang::jint prefixOffset,
        ::jxx::lang::jint prefixLength,
        ::jxx::lang::jlong contentLength);

    ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset, ::jxx::lang::jint length) override;
    ::jxx::lang::jlong skip(::jxx::lang::jlong count) override;
    ::jxx::lang::jint available() override;
    void close() override;
    ::jxx::lang::jbool isFullyConsumedInternal() const noexcept override;
    ::jxx::lang::jbool wasClosedInternal() const noexcept override;

private:
    ::jxx::Ptr<::jxx::io::InputStream> input_;
    ::jxx::lang::ByteArray prefix_;
    ::jxx::lang::jint prefixPosition_ = 0;
    ::jxx::lang::jint prefixLimit_ = 0;
    ::jxx::lang::jlong remaining_ = 0;
    ::jxx::lang::jbool closed_ = false;
};

} // namespace jxx::com::sun::net::httpserver::internal
