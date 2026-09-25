#pragma once

#include "io/jxx.io.IOException.h"

namespace jxx::ext::net::ssl {

class SSLException
    : public ::jxx::lang::ClassBase<
          SSLException,
          ::jxx::io::IOException> {
public:
    using JxxSuper = ::jxx::io::IOException;
    using Super = ::jxx::lang::ClassBase<SSLException, JxxSuper>;

    SSLException();
    explicit SSLException(const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit SSLException(const char* message);
    ~SSLException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::ext::net::ssl
