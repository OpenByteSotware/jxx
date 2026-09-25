#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "security/cert/jxx.security.cert.Certificate.h"
#include "security/jxx.security.Principal.h"

namespace jxx::ext::net::ssl {

class SSLSession : public ::jxx::lang::InterfaceBase<SSLSession> {
public:
    using CertificateArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::security::cert::Certificate>, 1U>;

    ~SSLSession() override = default;

    virtual ::jxx::lang::ByteArray getId() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getCipherSuite() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getProtocol() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getPeerHost() const = 0;
    virtual ::jxx::lang::jint getPeerPort() const = 0;

    virtual ::jxx::Ptr<CertificateArray> getLocalCertificates() const = 0;
    virtual ::jxx::Ptr<CertificateArray> getPeerCertificates() const = 0;

    virtual ::jxx::Ptr<::jxx::security::Principal>
    getPeerPrincipal() const = 0;

    virtual ::jxx::Ptr<::jxx::security::Principal>
    getLocalPrincipal() const = 0;

    virtual ::jxx::lang::jbool isValid() const = 0;
    virtual void invalidate() = 0;
};

} // namespace jxx::ext::net::ssl
