#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.SSLSession.h"
#include "util/jxx.util.List.h"

namespace jxx::ext::net::ssl {
class SNIServerName;
class ExtendedSSLSession
    : public ::jxx::lang::ClassBase<
          ExtendedSSLSession,
          ::jxx::lang::Object,
          SSLSession> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        ExtendedSSLSession, JxxSuper, SSLSession>;
    using StringArray = SSLSession::StringArray;
    ~ExtendedSSLSession() override = default;
    virtual ::jxx::Ptr<StringArray>
    getLocalSupportedSignatureAlgorithms() const = 0;
    virtual ::jxx::Ptr<StringArray>
    getPeerSupportedSignatureAlgorithms() const = 0;
    virtual ::jxx::Ptr<::jxx::util::List<SNIServerName>>
    getRequestedServerNames() const;
protected:
    ExtendedSSLSession() = default;
};
} // namespace jxx::ext::net::ssl
