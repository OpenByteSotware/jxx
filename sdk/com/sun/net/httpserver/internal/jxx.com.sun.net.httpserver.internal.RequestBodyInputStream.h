#pragma once

#include "io/jxx.io.InputStream.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::com::sun::net::httpserver::internal {

class RequestBodyInputStream final
    : public ::jxx::lang::ClassBase<RequestBodyInputStream, ::jxx::io::InputStream> {
public:
    using JxxSuper = ::jxx::io::InputStream;
    using Super = ::jxx::lang::ClassBase<RequestBodyInputStream, JxxSuper>;

    explicit RequestBodyInputStream(const ::jxx::lang::ByteArray& body);

    ::jxx::lang::jint read() override;
    ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset, ::jxx::lang::jint length) override;
    ::jxx::lang::jlong skip(::jxx::lang::jlong count) override;
    ::jxx::lang::jint available() override;
    void close() override;
    ::jxx::lang::jbool markSupported() const override;

    ::jxx::lang::jbool isFullyConsumedInternal() const noexcept;
    ::jxx::lang::jbool wasClosedInternal() const noexcept;

private:
    ::jxx::lang::ByteArray body_;
    ::jxx::lang::jint position_ = 0;
    ::jxx::lang::jbool closed_ = false;
};

} // namespace jxx::com::sun::net::httpserver::internal
