#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.HostnameVerifier.h"

namespace jxx::ext::net::ssl::internal {

class DefaultHostnameVerifier final
    : public ::jxx::lang::ClassBase<
          DefaultHostnameVerifier,
          ::jxx::lang::Object,
          ::jxx::ext::net::ssl::HostnameVerifier> {
public:
    ::jxx::lang::jbool verify(
        const ::jxx::Ptr<::jxx::lang::String>& hostname,
        const ::jxx::Ptr<::jxx::ext::net::ssl::SSLSession>& session) override;
};

} // namespace jxx::ext::net::ssl::internal
