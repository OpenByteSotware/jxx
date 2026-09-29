#pragma once
#include "ext/net/ssl/jxx.ext.net.ssl.ManagerFactoryParameters.h"
#include "security/cert/jxx.security.cert.CertPathParameters.h"
namespace jxx::ext::net::ssl {
class CertPathTrustManagerParameters final : public ::jxx::lang::ClassBase<CertPathTrustManagerParameters,::jxx::lang::Object,ManagerFactoryParameters>{
public:
 explicit CertPathTrustManagerParameters(const ::jxx::Ptr<::jxx::security::cert::CertPathParameters>& parameters);
 ::jxx::Ptr<::jxx::security::cert::CertPathParameters> getParameters() const;
private: ::jxx::Ptr<::jxx::security::cert::CertPathParameters> parameters_;
};}
